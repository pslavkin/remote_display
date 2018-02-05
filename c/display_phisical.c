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
//---------------------------------------------------------------------
void Write_Disp_Instr(unsigned char Instr)
{
	GPIOE->PCOR = 1<<1;	//Disp_DI_Clr();
	*((uint8_t *)(&GPIOA->PDOR)+1)=(uint8_t)(Instr);
	GPIOC->PCOR = 1<<7;	//Disp_WR_Clr();
	GPIOC->PSOR = 1<<7;	//Disp_WR_Set();
	GPIOE->PSOR = 1<<1;	//Disp_DI_Set();	//ya lo dejo en modo data.. que es lo que mas se usa..
}
void Write_Disp_Data(unsigned char Data)
{
	*((uint8_t *)(&GPIOA->PDOR)+1)=(uint8_t)(Data);
	GPIOC->PCOR = 1<<7;	//Disp_WR_Clr();
	GPIOC->PSOR = 1<<7;	//Disp_WR_Set();
}
void Write_Disp_2Data(uint16_t Data)
{
	*((uint8_t *)(&GPIOA->PDOR)+1)=(uint8_t)(Data>>8);
	GPIOC->PCOR = 1<<7;	//Disp_WR_Clr();
	GPIOC->PSOR = 1<<7;	//Disp_WR_Set();
	*((uint8_t *)(&GPIOA->PDOR)+1)=(uint8_t)(Data);
	GPIOC->PCOR = 1<<7;	//Disp_WR_Clr();
	GPIOC->PSOR = 1<<7;	//Disp_WR_Set();
}
unsigned char Read_Disp_Data(void)
{
	GPIOA->PCOR = 1<<16;	//Disp_RD_Clr();
	GPIOA->PSOR = 1<<16;	//Disp_RD_Set();
	return (GPIOA->PDIR&0x0000FF00)>>8;	//leo port
}
//----------------------------------------------------------------------
void Init_Lcd_Pins(void)/*{{{*/
{
    	CLOCK_EnableClock	(kCLOCK_PortA); //lo usa el canal de datos
    	CLOCK_EnableClock	(kCLOCK_PortC); //lo usan pines de seleccion
    	CLOCK_EnableClock	(kCLOCK_PortE); //lo usan pines de power

	//Contraste level 1 - CS pin 10
    	PORT_SetPinMux		(PORTE,14, kPORT_MuxAsGpio);	//
	GPIO_PinInit_As_Out	(GPIOE,14,0);			//chip select en cero siempre

	//power pin PTE1 - DI pin 11
    	PORT_SetPinMux		(PORTE, 1, kPORT_MuxAsGpio);	//
	GPIO_PinInit_As_Out	(GPIOE, 1,0);			// data = 1 command =0
	
	//D/I - /WR pin 12
    	PORT_SetPinMux		(PORTC, 7, kPORT_MuxAsGpio);	//write active low
	GPIO_PinInit_As_Out	(GPIOC, 7,1);			//

	//R/W /RD pin 13
    	PORT_SetPinMux		(PORTA,16, kPORT_MuxAsGpio);	//read active low
	GPIO_PinInit_As_Out	(GPIOA,16,0);			//

	//E1 - Reset mas IMO pin 30 y 31
    	PORT_SetPinMux		(PORTE, 6, kPORT_MuxAsGpio);	//en 1 reset se librea y se elije 8 bits...
	GPIO_PinInit_As_Out	(GPIOE, 6,0);			//estan en corto reset y IMO por que quedo practico..
	
	//BLIGHT2 - blighr a pin 34 y 35
    	PORT_SetPinMux		(PORTE, 16, kPORT_MuxAsGpio);	//
	GPIO_PinInit_As_Out	(GPIOE, 16,1);			//

	//D0-D7
    	PORT_SetPinMux		(PORTA,  8, kPORT_MuxAsGpio);	//
    	PORT_SetPinMux		(PORTA,  9, kPORT_MuxAsGpio);	//
    	PORT_SetPinMux		(PORTA, 10, kPORT_MuxAsGpio);	//
    	PORT_SetPinMux		(PORTA, 11, kPORT_MuxAsGpio);	//
    	PORT_SetPinMux		(PORTA, 12, kPORT_MuxAsGpio);	//
    	PORT_SetPinMux		(PORTA, 13, kPORT_MuxAsGpio);	//
    	PORT_SetPinMux		(PORTA, 14, kPORT_MuxAsGpio);	//
    	PORT_SetPinMux		(PORTA, 15, kPORT_MuxAsGpio);	//
	GPIO_Port_As_Out	(GPIOA,0x0000FF00); 
}/*}}}*/
void Disp_CS_Set	(void)			{GPIO_PortSet	(GPIOE,1<<14);}
void Disp_CS_Clr	(void)			{GPIO_PortClear	(GPIOE,1<<14);}
void Disp_DI_Set	(void)			{GPIO_PortSet	(GPIOE,1<< 1);}
void Disp_DI_Clr	(void)			{GPIO_PortClear	(GPIOE,1<< 1);}
void Disp_Rst_Set	(void)			{GPIO_PortSet	(GPIOE,1<< 6);}
void Disp_Rst_Clr	(void)			{GPIO_PortClear	(GPIOE,1<< 6);}
void Disp_WR_Set	(void)			{GPIO_PortSet	(GPIOC,1<< 7);}
void Disp_WR_Clr	(void)			{GPIO_PortClear	(GPIOC,1<< 7);}
void Disp_RD_Set	(void)			{GPIO_PortSet	(GPIOA,1<<16);}
void Disp_RD_Clr	(void)			{GPIO_PortClear	(GPIOA,1<<16);}
//----------------------------------------------------------------------
void Init_Display_Phisical(void)/*{{{*/
{
        Init_Lcd_Pins();
	Disp_CS_Clr();
	Disp_RD_Set();
	Disp_WR_Clr();
	Disp_Rst_Clr();
	Write_Disp_Instr(0x28);	//display Off  	//testing
	Write_Disp_Data(0x00);			//testing
        Delay_Useg(1000);
	Disp_Rst_Set();		//esto es porque podria filtrarse la senial de reset sino.
        Delay_Useg(120000);	//hay que esperar 120mseg.. ver pag 230 del controlador

	Write_Disp_Instr(0x11);	//exit SLEEP mode  /arranca en sleep in..
        Delay_Useg(5000);	//esperar 5mseg


	Write_Disp_Instr(0xCB);	//Power Control A 
	Write_Disp_Data(0x39);	//always 0x39
	Write_Disp_Data(0x2C);	//always 0x2C 
	Write_Disp_Data(0x00);	//always 0x00 
	Write_Disp_Data(0x34);	//Vcore = 1.6V 
	Write_Disp_Data(0x02);	//DDVDH = 5.6V 

	Write_Disp_Instr(0xCF);	//Power Control B 
	Write_Disp_Data(0x00);	//always 0x00 
	Write_Disp_Data(0x81);	//PCEQ off 
	Write_Disp_Data(0x30);	//ESD protection 

	Write_Disp_Instr(0xE8);	//Driver timing control A 
	Write_Disp_Data(0x85);	//non‐overlap 
	Write_Disp_Data(0x01);	//EQ timing 
	Write_Disp_Data(0x79);	//Pre‐charge timing 

	Write_Disp_Instr(0xEA);	//Driver timing control B 
	Write_Disp_Data(0x00);	//Gate driver timing 
	Write_Disp_Data(0x00);	//always 0x00 

	Write_Disp_Instr(0xED);	//Power‐On sequence control 
	Write_Disp_Data(0x64);	//soft start 
	Write_Disp_Data(0x03);	//power on sequence 
	Write_Disp_Data(0x12);	//power on sequence 
	Write_Disp_Data(0x81);	//DDVDH enhance on 

	Write_Disp_Instr(0xF7);	//Pump ratio control 
	Write_Disp_Data(0x20);	//DDVDH=2xVCI 
	 
	Write_Disp_Instr(0xC0);	//power control 1 
	Write_Disp_Data(0x26);
	 
	Write_Disp_Instr(0xC1);	//power control 2 
	Write_Disp_Data(0x11);
	 
	Write_Disp_Instr(0xC5);	//VCOM control 1 
	Write_Disp_Data(0x35);
	Write_Disp_Data(0x3E);

	Write_Disp_Instr(0xC7);	//VCOM control 2 
	Write_Disp_Data(0xBE); 

	Write_Disp_Instr(0x36);	//memory access control = BGR 
	Write_Disp_Data(0x88);

	Write_Disp_Instr(0xB1);	//frame rate control 
	Write_Disp_Data(0x00);
	Write_Disp_Data(0x10);

	Write_Disp_Instr(0xB6);	//display function control 
	Write_Disp_Data(0x0A);
	Write_Disp_Data(0xA2);

	Write_Disp_Instr(0x3A);	//pixel format = 16 bit per pixel 
	Write_Disp_Data(0x55);

	Write_Disp_Instr(0xF2);	//3G Gamma control 
	Write_Disp_Data(0x02);	//off 

	Write_Disp_Instr(0x26);	//Gamma curve 3 
	Write_Disp_Data(0x01);
 
	Clear_Lcd();

	Write_Disp_Instr(0x29);	//display On 
	Write_Disp_Data(0x00);	

	///Clear_Lcd();
}/*}}}*/
void Set_Frame_Address(struct Struct_Pic *Pic)
{
	Write_Disp_Instr(0x2A);			//column address set 
	Write_Disp_2Data(Pic->Start_X);
	Write_Disp_2Data(Pic->Start_X+Pic->Width);
	 
	Write_Disp_Instr(0x2B);		//page address set 
	Write_Disp_2Data(Pic->Start_Y);
	Write_Disp_2Data(Pic->Start_Y+Pic->Height);
}
void Clear_Lcd(void)
{
	struct Struct_Pic Pic={0,239,0,319,0,0,0,NULL,0,NULL};
	Set_Frame_Address(&Pic);
	Write_Disp_Instr(0x2C);	
	for(int j=0;j<320;j++) 
		for(int i=0;i<240;i++) 
			Write_Disp_2Data(0);
}
//----------------------------------------------------------------------
uint16_t Invert_Pixel(uint16_t Pixel)
{
	uint8_t R=(Pixel&0xF800)>>11;
	uint8_t G=(Pixel&0x07E0)>>5;
	uint8_t B=(Pixel&0x001F)>>0;
	R=0x1F-R;
	G=0x3F-G;
	B=0x1F-B;
	return (R<<11 | G<<5 | B);
}
void Lcd2Pic_Inverted(struct Struct_Pic *Pic) 
{
	uint8_t R,G,B;
	uint32_t Size=Pic->Width*Pic->Height; 
	Set_Frame_Address(Pic);
	Write_Disp_Instr(0x2E);			//comando de lectura
	GPIO_Port_As_In(GPIOA,0x0000FF00);
	Read_Disp_Data();			//dummy read..
	for(uint32_t i=0;i<Size;i++) {
		R=0x1F-(Read_Disp_Data()>>3);	//WTF??? sip.. le mando de a 2 bytes y me devuelve 3, y solo usa la parte alta del byte
		G=0x3F-(Read_Disp_Data()>>2);
		B=0x1F-(Read_Disp_Data()>>3);
		Pic->Data[0][i]=R<<11 | G<<5 | B;
	}
	GPIO_Port_As_Out(GPIOA,0x0000FF00);
}


uint8_t Actual_Sub_Pic_Index;
struct Struct_Pic Actual_Sub_Pic;

void Pic2Lcd(struct Struct_Pic *Pic) 
{
	Actual_Sub_Pic=*Pic;
	Actual_Sub_Pic_Index=0;
	Sub_Pic2Lcd();
}
void Sub_Pic2Lcd(void) 
{
	if(Actual_Sub_Pic_Index<Actual_Sub_Pic.PCount) {
		Set_Frame_Address(&Actual_Sub_Pic);
		Write_Disp_Instr(0x2C);	
		Pic2TCD(&Actual_Sub_Pic,Actual_Sub_Pic_Index);
		Actual_Sub_Pic.Start_X+=Actual_Sub_Pic.Width;
		Actual_Sub_Pic_Index++;
	} else 
		Atomic_Send_Event(Next_Layer_Event,Display_Layers());
}

