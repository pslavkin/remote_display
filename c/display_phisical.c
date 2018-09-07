#include "pin_mux.h"
#include "clock_config.h"
#include "mke18f16.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "everythings.h"
#include "state_machine.h"
#include "events.h"
#include "serial_tx.h"
#include "schedule.h"
#include "leds_session.h"
#include "display_phisical.h"
#include "display_pics.h"
#include "display_layers.h"
#include "str.h"
#include "dma.h"
#include <stdint.h>
#include "type_conversion.h"
#include "serial_tx.h"
#include "ftm.h"
//---------------------------------------------------------------------
uint8_t           Actual_Sub_Pic_Index;
struct Struct_Pic Actual_Sub_Pic;
//---------------------------------------------------------------------
void Write_Disp_Instr(uint8_t Instr)
{
   GPIOD->PCOR = 1<<3;  //Disp_DI_Clr();
   *((uint8_t *)(&GPIOB->PDOR)+0)=Instr;
   GPIOA->PCOR = 1<<2;  //Disp_WR_Clr();
   GPIOA->PSOR = 1<<2;  //Disp_WR_Set();
   GPIOD->PSOR = 1<<3;  //Disp_DI_Set();  //ya lo dejo en modo data.. que es lo que mas se usa..
}
void Write_Disp_16b_Data(uint16_t Data)
{
   *(uint16_t *)(&GPIOB->PDOR)=Data;
   GPIOA->PCOR = 1<<2;  //Disp_WR_Clr();
   GPIOA->PSOR = 1<<2;  //Disp_WR_Set();
}

void Write_Disp_8b_Param(uint8_t Data)
{
   *((uint8_t *)(&GPIOB->PDOR)+0)=Data;
   GPIOA->PCOR = 1<<2;  //Disp_WR_Clr();
   GPIOA->PSOR = 1<<2;  //Disp_WR_Set();
}
void Write_Disp_16b_Param(uint16_t Data)
{
   *((uint8_t *)(&GPIOB->PDOR)+0)=(uint8_t)(Data>>8);
   GPIOA->PCOR = 1<<2;  //Disp_WR_Clr();
   GPIOA->PSOR = 1<<2;  //Disp_WR_Set();
   *((uint8_t *)(&GPIOB->PDOR)+0)=(uint8_t)(Data);
   GPIOA->PCOR = 1<<2;  //Disp_WR_Clr();
   GPIOA->PSOR = 1<<2;  //Disp_WR_Set();
}
uint16_t Read_Disp_Data(void)
{
   GPIOA->PCOR = 1<< 3; //Disp_RD_Clr();
   GPIOA->PSOR = 1<< 3; //Disp_RD_Set();
   return (uint16_t) (GPIOB->PDIR&0x0000FFFF); //leo port
}
//----------------------------------------------------------------------
void Init_Lcd_Pins(void)/*{{{*/
{
      CLOCK_EnableClock ( kCLOCK_PortA ); // lo usan pines de seleccion
      CLOCK_EnableClock ( kCLOCK_PortB ); // lo usa el canal de datos y reset y IMO
//      CLOCK_EnableClock ( kCLOCK_PortC ); // lo usa bligth por ahora...
      CLOCK_EnableClock ( kCLOCK_PortD ); // lo usan pines de seleccion

      // CS port PTD2 - pin 71
      PORT_SetPinMux      ( PORTD, 2, kPORT_MuxAsGpio ); //
      GPIO_PinInit_As_Out ( GPIOD, 2,0                ); // chip select en cero siempre

      //DC port  PTD3 - pin 71
      PORT_SetPinMux      ( PORTD, 3, kPORT_MuxAsGpio ); //
      GPIO_PinInit_As_Out ( GPIOD, 3,0                ); // data = 1 command =0

      //WR port PTA2 - pin 73
      PORT_SetPinMux      ( PORTA, 2, kPORT_MuxAsGpio ); // write active low
      GPIO_PinInit_As_Out ( GPIOA, 2,1                ); //

      //RD port PTA3 - pin 72
      PORT_SetPinMux      ( PORTA, 3, kPORT_MuxAsGpio ); // read active low
      GPIO_PinInit_As_Out ( GPIOA, 3,0                ); //

      //RST port PTB17 - pin 63
      PORT_SetPinMux      ( PORTB,17, kPORT_MuxAsGpio ); // en 1 reset se librea 
      GPIO_PinInit_As_Out ( GPIOB,17,0                ); //

      //IMO port PTB16 - pin 64
      PORT_SetPinMux      ( PORTB,16, kPORT_MuxAsGpio ); // en 1 se elije 8 bits...
      GPIO_PinInit_As_Out ( GPIOB,16,0                ); //
      //
//manejo el bligth con pwm tpm0ch0
//      //BLIGHT port PTC0 - pin 40 (por ahora como gpio.. pero tiene que tener FTM
//      PORT_SetPinMux      ( PORTC,  0, kPORT_MuxAsGpio ); //
//      GPIO_PinInit_As_Out ( GPIOC,  0,1                ); //

   //D0-D7
      PORT_SetPinMux   ( PORTB,  0 , kPORT_MuxAsGpio ); //
      PORT_SetPinMux   ( PORTB,  1 , kPORT_MuxAsGpio ); //
      PORT_SetPinMux   ( PORTB,  2 , kPORT_MuxAsGpio ); //
      PORT_SetPinMux   ( PORTB,  3 , kPORT_MuxAsGpio ); //
      PORT_SetPinMux   ( PORTB,  4 , kPORT_MuxAsGpio ); //
      PORT_SetPinMux   ( PORTB,  5 , kPORT_MuxAsGpio ); //
      PORT_SetPinMux   ( PORTB,  6 , kPORT_MuxAsGpio ); //
      PORT_SetPinMux   ( PORTB,  7 , kPORT_MuxAsGpio ); //
      PORT_SetPinMux   ( PORTB,  8 , kPORT_MuxAsGpio ); //
      PORT_SetPinMux   ( PORTB,  9 , kPORT_MuxAsGpio ); //
      PORT_SetPinMux   ( PORTB,  10, kPORT_MuxAsGpio ); //
      PORT_SetPinMux   ( PORTB,  11, kPORT_MuxAsGpio ); //
      PORT_SetPinMux   ( PORTB,  12, kPORT_MuxAsGpio ); //
      PORT_SetPinMux   ( PORTB,  13, kPORT_MuxAsGpio ); //
      PORT_SetPinMux   ( PORTB,  14, kPORT_MuxAsGpio ); //
      PORT_SetPinMux   ( PORTB,  15, kPORT_MuxAsGpio ); //
      GPIO_Port_As_Out ( GPIOB,0x0000FFFF           );
}/*}}}*/
void Disp_CS_Set  ( void ) { GPIO_PortSet   ( GPIOD,1<< 2 );}
void Disp_CS_Clr  ( void ) { GPIO_PortClear ( GPIOD,1<< 2 );}
void Disp_DI_Set  ( void ) { GPIO_PortSet   ( GPIOD,1<< 3 );}
void Disp_DI_Clr  ( void ) { GPIO_PortClear ( GPIOD,1<< 3 );}
void Disp_WR_Set  ( void ) { GPIO_PortSet   ( GPIOA,1<< 2 );}
void Disp_WR_Clr  ( void ) { GPIO_PortClear ( GPIOA,1<< 2 );}
void Disp_RD_Set  ( void ) { GPIO_PortSet   ( GPIOA,1<< 3 );}
void Disp_RD_Clr  ( void ) { GPIO_PortClear ( GPIOA,1<< 3 );}
void Disp_Rst_Set ( void ) { GPIO_PortSet   ( GPIOB,1<<17 );}
void Disp_Rst_Clr ( void ) { GPIO_PortClear ( GPIOB,1<<17 );}
void Disp_Imo_Set ( void ) { GPIO_PortSet   ( GPIOB,1<<16 );}
void Disp_Imo_Clr ( void ) { GPIO_PortClear ( GPIOB,1<<16 );}
//----------------------------------------------------------------------
void Init_Display_Phisical_9341(void)/*{{{*/
{
   Init_Lcd_Pins       (        );
   Disp_CS_Clr         (        );
   Disp_RD_Set         (        );
   Disp_WR_Clr         (        );
   Disp_Rst_Clr        (        );
   Disp_Imo_Set        (        );
   Delay_Useg          ( 1000   );
   Disp_Rst_Set        (        ); // esto es porque podria filtrarse la senial de reset sino.
   Delay_Useg          ( 120000 ); // hay que esperar 120mseg.. ver pag 230 del controlador

   Write_Disp_Instr    ( 0x28   ); // display Off     //testing
   Write_Disp_8b_Param ( 0x00   ); // testing
   Write_Disp_Instr    ( 0x11   ); // exit SLEEP mode  /arranca en sleep in..
   Delay_Useg          ( 5000   ); // esperar 5mseg


   Write_Disp_Instr    ( 0xCB   ); // Power Control A
   Write_Disp_8b_Param ( 0x39   ); // always 0x39
   Write_Disp_8b_Param ( 0x2C   ); // always 0x2C
   Write_Disp_8b_Param ( 0x00   ); // always 0x00
   Write_Disp_8b_Param ( 0x34   ); // Vcore = 1.6V
   Write_Disp_8b_Param ( 0x02   ); // DDVDH = 5.6V

   Write_Disp_Instr    ( 0xCF   ); // Power Control B
   Write_Disp_8b_Param ( 0x00   ); // always 0x00
   Write_Disp_8b_Param ( 0x81   ); // PCEQ off
   Write_Disp_8b_Param ( 0x30   ); // ESD protection

   Write_Disp_Instr    ( 0xE8   ); // Driver timing control A
   Write_Disp_8b_Param ( 0x85   ); // non‐overlap
   Write_Disp_8b_Param ( 0x01   ); // EQ timing
   Write_Disp_8b_Param ( 0x79   ); // Pre‐charge timing

   Write_Disp_Instr    ( 0xEA   ); // Driver timing control B
   Write_Disp_8b_Param ( 0x00   ); // Gate driver timing
   Write_Disp_8b_Param ( 0x00   ); // always 0x00

   Write_Disp_Instr    ( 0xED   ); // Power‐On sequence control
   Write_Disp_8b_Param ( 0x64   ); // soft start
   Write_Disp_8b_Param ( 0x03   ); // power on sequence
   Write_Disp_8b_Param ( 0x12   ); // power on sequence
   Write_Disp_8b_Param ( 0x81   ); // DDVDH enhance on

   Write_Disp_Instr    ( 0xF7   ); // Pump ratio control
   Write_Disp_8b_Param ( 0x20   ); // DDVDH=2xVCI

   Write_Disp_Instr    ( 0xC0   ); // power control 1
   Write_Disp_8b_Param ( 0x26   );

   Write_Disp_Instr    ( 0xC1   ); // power control 2
   Write_Disp_8b_Param ( 0x11   );

   Write_Disp_Instr    ( 0xC5   ); // VCOM control 1
   Write_Disp_8b_Param ( 0x35   );
   Write_Disp_8b_Param ( 0x3E   );

   Write_Disp_Instr    ( 0xC7   ); // VCOM control 2
   Write_Disp_8b_Param ( 0xBE   );

   Write_Disp_Instr    ( 0x36   ); // memory access control = BGR
   Write_Disp_8b_Param ( 0x88   );

   Write_Disp_Instr    ( 0xB1   ); // frame rate control
   Write_Disp_8b_Param ( 0x00   );
   Write_Disp_8b_Param ( 0x10   );

   Write_Disp_Instr    ( 0xB6   ); // display function control
   Write_Disp_8b_Param ( 0x0A   );
   Write_Disp_8b_Param ( 0xA2   );

   Write_Disp_Instr    ( 0x3A   ); // pixel format = 16 bit per pixel
   Write_Disp_8b_Param ( 0x55   );

   Write_Disp_Instr    ( 0xF2   ); // 3G Gamma control
   Write_Disp_8b_Param ( 0x02   ); // off

   Write_Disp_Instr    ( 0x26   ); // Gamma curve 3
   Write_Disp_8b_Param ( 0x01   );

//   Clear_Lcd           (        );

   Write_Disp_Instr    ( 0x29   ); // display On
   Write_Disp_8b_Param ( 0x00   );

}                                  /* }}}*/
void Init_Display_Phisical_7789(void) /*  {{{*/
{
   Init_Lcd_Pins       (        );
   Disp_CS_Clr         (        );
   Disp_RD_Set         (        );
   Disp_WR_Set         (        );
   Disp_Rst_Clr        (        );
   Disp_Imo_Clr        (        ); // 16 bit de datos
   Delay_Useg          ( 1000   );
   Disp_Rst_Set        (        ); // esto es porque podria filtrarse la senial de reset sino.
   Delay_Useg          ( 120000 ); // hay que esperar 120mseg.. ver pag 230 del controlador

   Write_Disp_Instr    ( 0x11   ); // exit SLEEP mode  /arranca en sleep in..
   Delay_Useg          ( 5000   ); // esperar 5mseg


   Write_Disp_Instr    ( 0x36   );
   Write_Disp_8b_Param ( 0x00   ); // MADCTL: memory data access control
   Write_Disp_Instr    ( 0x3A   );
   Write_Disp_8b_Param ( 0x55   ); // COLMOD: Interface Pixel format
   Write_Disp_Instr    ( 0xB2   );
   Write_Disp_8b_Param ( 0x0C   );
   Write_Disp_8b_Param ( 0x0C   );
   Write_Disp_8b_Param ( 0x00   );
   Write_Disp_8b_Param ( 0x33   );
   Write_Disp_8b_Param ( 0x33   ); // PORCTRK: Porch setting
   Write_Disp_Instr    ( 0xB7   );
   Write_Disp_8b_Param ( 0x35   ); // GCTRL: Gate Control
   Write_Disp_Instr    ( 0xBB   );
   Write_Disp_8b_Param ( 0x2B   ); // VCOMS: VCOM setting
   Write_Disp_Instr    ( 0xC0   );
   Write_Disp_8b_Param ( 0x2C   ); // LCMCTRL: LCM Control
   Write_Disp_Instr    ( 0xC2   );
   Write_Disp_8b_Param ( 0x01   );
   Write_Disp_8b_Param ( 0xFF   ); // VDVVRHEN: VDV and VRH Command Enable
   Write_Disp_Instr    ( 0xC3   );
   Write_Disp_8b_Param ( 0x11   ); // VRHS: VRH Set
   Write_Disp_Instr    ( 0xC4   );
   Write_Disp_8b_Param ( 0x20   ); // VDVS: VDV Set
   Write_Disp_Instr    ( 0xC6   );
   Write_Disp_8b_Param ( 0x0F   ); // FRCTRL2: Frame Rate control in normal mode
   Write_Disp_Instr    ( 0xD0   );
   Write_Disp_8b_Param ( 0xA4   );
   Write_Disp_8b_Param ( 0xA1   ); // PWCTRL1: Power Control 1
   Write_Disp_Instr    ( 0xE0   );
   Write_Disp_8b_Param ( 0xD0   );
   Write_Disp_8b_Param ( 0x00   );
   Write_Disp_8b_Param ( 0x05   );
   Write_Disp_8b_Param ( 0x0E   );
   Write_Disp_8b_Param ( 0x15   );
   Write_Disp_8b_Param ( 0x0D   );
   Write_Disp_8b_Param ( 0x37   );
   Write_Disp_8b_Param ( 0x43   );
   Write_Disp_8b_Param ( 0x47   );
   Write_Disp_8b_Param ( 0x09   );
   Write_Disp_8b_Param ( 0x15   );
   Write_Disp_8b_Param ( 0x12   );
   Write_Disp_8b_Param ( 0x16   );
   Write_Disp_8b_Param ( 0x19   ); // PVGAMCTRL: Positive Voltage Gamma control
   Write_Disp_Instr    ( 0xE1   );
   Write_Disp_8b_Param ( 0xD0   );
   Write_Disp_8b_Param ( 0x00   );
   Write_Disp_8b_Param ( 0x05   );
   Write_Disp_8b_Param ( 0x0D   );
   Write_Disp_8b_Param ( 0x0C   );
   Write_Disp_8b_Param ( 0x06   );
   Write_Disp_8b_Param ( 0x2D   );
   Write_Disp_8b_Param ( 0x44   );
   Write_Disp_8b_Param ( 0x40   );
   Write_Disp_8b_Param ( 0x0E   );
   Write_Disp_8b_Param ( 0x1C   );
   Write_Disp_8b_Param ( 0x18   );
   Write_Disp_8b_Param ( 0x16   );
   Write_Disp_8b_Param ( 0x19   ); // NVGAMCTRL: Negative Voltage Gamma control
   Write_Disp_Instr    ( 0x2A   );
   Write_Disp_8b_Param ( 0x00   );
   Write_Disp_8b_Param ( 0x00   );
   Write_Disp_8b_Param ( 0x00   );
   Write_Disp_8b_Param ( 0xEF   ); // X address set
   Write_Disp_Instr    ( 0x2B   );
   Write_Disp_8b_Param ( 0x00   );
   Write_Disp_8b_Param ( 0x00   );
   Write_Disp_8b_Param ( 0x01   );
   Write_Disp_8b_Param ( 0x3F   ); // Y address set

//   Clear_Lcd           (        ); // opcional

   Write_Disp_Instr    ( 0x29   ); // display On
   Write_Disp_8b_Param ( 0x00   );

}                                  /*  }}}*/
void Set_Frame_Address(struct Struct_Pic *Pic)
{
   Write_Disp_Instr     ( 0x2A        ); // column address set
   Write_Disp_16b_Param ( Pic->Pos.XL );
   Write_Disp_16b_Param ( Pic->Pos.XR );

   Write_Disp_Instr     ( 0x2B        ); // page address set
   Write_Disp_16b_Param ( Pic->Pos.YU );
   Write_Disp_16b_Param ( Pic->Pos.YL );
}
void Write_Pic(struct Struct_Pic* Pic)
{
   uint32_t Size=Pic_Area ( Pic  );
   Set_Frame_Address ( Pic );
   Write_Disp_Instr  ( 0x2C );
   for(uint32_t i=0;i<Size;i++)
         Write_Disp_16b_Data ( Pic->Data[0][i]);
}
void Read_Lcd(struct Struct_Pic* Pic,bool Invert)
{
   Write_Disp_Instr       ( 0x3A );
   Write_Disp_8b_Param    ( 0x66 );   // COLMOD: Interface Pixel format
   uint32_t Size=Pic_Area ( Pic  );

   Set_Frame_Address ( Pic              );
   Write_Disp_Instr  ( 0x2E             ); // comando de lectura
   GPIO_Port_As_In   ( GPIOB,0x0000FFFF );
   Read_Disp_Data    (                  ); // dummy read..
   for(uint32_t i=0;i<Size;i+=2) {        //cuento de a 2 porque en cada loop genero info para 2 pixeles
      uint16_t R,G,B;
      uint16_t D1,D2,D3;
      D1=Read_Disp_Data();
      D2=Read_Disp_Data();
      D3=Read_Disp_Data();

      R=( (D1&0x7E00 )>>10);
      G=( (D1&0x00FC )>>2 );
      B=( (D2&0x7E00 )>>10);
      if(Invert) {
         R=0x1F-R;
         G=0x3F-G;
         B=0x1F-B;
      }
      Pic->Data[0][i+0]=R<<11 | G<<5 | B;

      R=((D2&0x00FC)>>3);
      G=((D3&0x7E00)>>9);
      B=((D3&0x00FC)>>3);
      if(Invert) {
         R=0x1F-R;
         G=0x3F-G;
         B=0x1F-B;
      }
      Pic->Data[0][i+1]=R<<11 | G<<5 | B;
   }
   GPIO_Port_As_Out    ( GPIOB,0x0000FFFF );
   Write_Disp_Instr    ( 0x3A             );
   Write_Disp_8b_Param ( 0x55             ); // COLMOD: Interface Pixel format
}
void Clear_Lcd(void)
{
   struct Struct_Pic Pic={{0,239,0,319},0,0,0,NULL,0,NULL};
   Paint_Lcd(&Pic,0x0000);
}
void Paint_Lcd(struct Struct_Pic* Pic,uint16_t Bkgd)
{
   uint32_t Size=Pic_Area ( Pic  );
   Set_Frame_Address ( Pic );
   Write_Disp_Instr  ( 0x2C );
   for(uint32_t i=0;i<Size;i++)
         Write_Disp_16b_Data ( Bkgd );
}
//----------------------------------------------------------------------
void Invert_Pic(struct Struct_Pic *Pic)   //lee fila a fila y la invierte (para hacer una mascara de cualquier tamaio con un stack de 240 words)
{
   uint16_t Data[240];
   uint16_t *Data_Raw[]={Data};
   struct Struct_Pic Row_Pic= { { 0 ,0 ,0 ,0 } ,0 ,0 ,0 ,Rien_Events ,1 ,Data_Raw };
   Row_Pic.Pos.XL=Pic->Pos.XL;
   Row_Pic.Pos.XR=Pic->Pos.XR;
   for(uint32_t i=Pic->Pos.YU;i<Pic->Pos.YL;i++) {
      Row_Pic.Pos.YU=Row_Pic.Pos.YL=i;
      Read_Lcd  ( &Row_Pic,true );     //true porque invierto antes de cuardar...
      Write_Pic ( &Row_Pic      );     //escribo la misma pic invertida.
   }
}
//--------------------------------------------------------------------------------
void Pic2Lcd(struct Struct_Pic *Pic)
{
   Actual_Sub_Pic       = *Pic;
   Actual_Sub_Pic_Index = 0;
   Sub_Pic2Lcd();
}
void Sub_Pic2Lcd(void)
{
   if(Actual_Sub_Pic_Index<Actual_Sub_Pic.PCount) {
      Set_Frame_Address(&Actual_Sub_Pic);
      Write_Disp_Instr(0x2C);
      Pic2TCD_Mod(&Actual_Sub_Pic,Actual_Sub_Pic_Index,Actual_Sub_Pic.Mod);
      uint16_t Width=Pic_Width(&Actual_Sub_Pic);
      Actual_Sub_Pic.Pos.XL+=Width;
      Actual_Sub_Pic.Pos.XR+=Width;
      Actual_Sub_Pic_Index++;
   } else
      Atomic_Send_Event(Next_Layer_Event,Display_Layers());
}

