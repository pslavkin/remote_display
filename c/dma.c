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
struct TCD_Struct TCD[6] __attribute__((aligned (32)));

void Pic2TCD(struct Struct_Pic *Pic,uint8_t Index)
{
	uint32_t Pic_Size=(Pic->Width+1)*(Pic->Height+1)*2;
	uint32_t Pos=0;
	uint16_t TCD_Size;
	for(uint8_t i=0; i<6 && Pic_Size>0 ;i++) {
		TCD[i].SADDR=(uint32_t)(((uint8_t *)(Pic->Data[Index]))+Pos);
		TCD[i].SOFF=1;
		TCD[i].ATTR=0;
		TCD[i].NBYTES_MLNO=1;
		TCD[i].SLAST=0;
		TCD[i].DADDR=(uint32_t) ((uint8_t *)(&GPIOA->PDOR)+1);
		TCD[i].DOFF=0;
		if(Pic_Size>0x7FFF) {
			TCD_Size=0x7FFF;
			Pic_Size-=0x7FFF;
			TCD[i].CSR=0x0010;		//
		}
		else {
			TCD_Size=Pic_Size;
			Pic_Size=0;
			TCD[i].CSR=0x000A;		//
		}
		Pos+=TCD_Size;
		TCD[i].DLAST_SGA=(uint32_t)&TCD[i+1];
		TCD[i].CITER_ELINKNO=TCD[i].BITER_ELINKNO=TCD_Size;
	}
	DMA0->TCD[0].CSR&=~0x0080;		//tip! hay que borrar el bit DONE de un previo dma complete para que me acepte escribir datos en este registro... sino no lo hace y no me linkea los TCD 
	DMA0->TCD[0]=TCD[0];
	DMA0->SSRT=0;			//what? arranco la primera adquisizion para que haya algun dato valido en el bus ANTES de que se mueve WR y meta fruta....
	for(uint16_t i=0;i<1;i++);	//hay que esperar un poquito para darle tiempo a que cargue el dato el primer tiro de DMA
	FTM3->CONTROLS[3].CnSC&=~0x00000080;
	FTM3->CNT=0;
	PORTC->PCR[7]=0x00000400;	// para no llamar a una funcion que tarda mucho como:	PORT_SetPinMux		(PORTC, 7, kPORT_MuxAlt4);	//write active low
	DMA0->ERQ|=0x0001;		//este es el que activa el request
}
void Init_Dma(void)
{
	Dma_Sm=Off;
	CLOCK_EnableClock(kCLOCK_Dmamux0);
	CLOCK_EnableClock(kCLOCK_Dma0);
	
	DMAMUX->CHCFG[0]=35;		//configura el source del canal cero como la fuente cero que es siempre enable
	DMAMUX->CHCFG[0]|=0x80;		//con este bit prendo 
	NVIC_EnableIRQ(DMA0_IRQn);
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
    	PORT_SetPinMux		(PORTC, 7, kPORT_MuxAsGpio);	//write active low
	Set_Temp_Led_Effect(Led_Run,0xFFFF);
	Send_Event(Next_Sub_Pic_Event,Display_Layers());
}
void Print_Destin(void)
{
}
State** 	Dma		(void)		{return &Dma_Sm;}
void 		Dma_Rti		(void)		{}
//-------------------------------------------------------------------
static State Off[] RODATA =
{
 {ANY_Event		,Rien		,Off},
};
//-------------------------------------------------------------------------------
