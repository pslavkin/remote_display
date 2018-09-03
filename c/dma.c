#include "mke18f16.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "everythings.h"
#include "leds_session.h"
#include "state_machine.h"
#include "events.h"
#include "schedule.h"
#include "dma.h"
#include "type_conversion.h"
#include "str.h"
#include "serial_tx.h"
#include "display_phisical.h"
#include "display_pics.h"
#include "display_layers.h"
#include "welcome_pic.h"

static  State
   Off[];

State *Dma_Sm;
//-------------------------------------------------------------------
struct TCD_Struct TCD[3] __attribute__((aligned (32))); //320*240=76800 words de 16b. cada TCD puede guard 0x7FFF=32767 => con 3=98301 entre la pic mas grande

void Pic2TCD(struct Struct_Pic *Pic,uint8_t Index)
{
   uint32_t Pic_Size=Pic_Area(Pic); //mas uno al ancho y al largo porque estan definidas asi, de 0 a ese valor. y el +1 al final, es porque se arega 2 byte extra, uno porque como el primer byte lo cargo con disparo manual no tiene asociado un disparo de ftm propio, y el otro es porque el ultimo byte no entra ya que cuando termina de cargar el ultimo byte, corto el FTM...
   uint32_t Pos=0;
   uint16_t TCD_Size;
   for(uint8_t i=0; i<6 && Pic_Size>0 ;i++) {
      TCD[i].SADDR       = (uint32_t)(Pic->Data[Index]+Pos);
      TCD[i].SOFF        = 2;
      TCD[i].ATTR        = 0x0101; //16 bits transfer
      TCD[i].NBYTES_MLNO = 2;
      TCD[i].SLAST       = 0;
      TCD[i].DADDR       = (uint32_t)(&GPIOB->PDOR);
      TCD[i].DOFF        = 0;
      if(Pic_Size>0x7FFF) {
         TCD_Size    = 0x7FFF;
         Pic_Size   -= 0x7FFF;
         TCD[i].CSR  = 0x0010;
      }
      else {
         TCD_Size   = Pic_Size;
         Pic_Size   = 0       ;
         TCD[i].CSR = 0x000A  ;
      }
      Pos                  += TCD_Size                       ;
      TCD[i].DLAST_SGA      = ( uint32_t )&TCD[i+1]          ;
      TCD[i].CITER_ELINKNO  = TCD[i].BITER_ELINKNO = TCD_Size;
   }
   DMA0->TCD[0].CSR &= ~0x0080     ; // tip! hay que borrar el bit DONE de un previo dma complete para que me acepte escribir datos en este registro... sino no lo hace y no me linkea los TCD
   DMA0->TCD[0]      = TCD[0]      ;
   //GPIOB->PDOR       = Pic->Data[0][0];
   FTM3->CNT         = 0           ;
   DMA0->SERQ        = 0x00        ; // este es el que activa el request
   PORTA->PCR[2]     = 0x00000200  ; // PORT_SetPinMux    (PORTA, 2, kPORT_MuxAlt2); //write active low
   FTM3->CNT         = 0           ;
   FTM3->CNT         = 0           ;
   FTM3->CNT         = 0           ;
}
void Init_Dma(void)
{
   Dma_Sm=Off;
   CLOCK_EnableClock ( kCLOCK_Dmamux0 );
   CLOCK_EnableClock ( kCLOCK_Dma0    );

   DMAMUX->CHCFG[0]  = 32;   // configura el source del canal cero como la fuente 32 que es el FTM3CH0
   DMAMUX->CHCFG[0] |= 0x80; // con este bit prendo (se podria haber puesto en una sola instruccion.. probar)
   NVIC_EnableIRQ(DMA0_IRQn); //la iRQ salta cuando termina de transferir todo el pic y recien ahi agpgo el ftm y aviso con un evento

}

void Dma_Clear(void)
{
}
void Dma_Request(void)
{
}
void DMA0_IRQHandler(void)
{
   DMA0->CINT=0;
   PORT_SetPinMux      ( PORTA, 2, kPORT_MuxAsGpio          ) ; //paso el pin como gipo en vez de ftm3 
   Set_Temp_Led_Effect ( Led_Run,0xFFFF                     ) ;
   Send_Event          ( Next_Sub_Pic_Event,Display_Layers( ));
}
void Print_Destin(void)
{
}
State**  Dma     ( void ) { return &Dma_Sm;}
void     Dma_Rti ( void ) { }
//-------------------------------------------------------------------
static State Off[] RODATA =
{
 {ANY_Event    ,Rien    ,Off},
};
//-------------------------------------------------------------------------------
