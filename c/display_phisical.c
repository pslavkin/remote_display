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
#include "str.h"
#include <stdint.h>
#include "type_conversion.h"

char* Clear_Data RODATA="                    ";
uint8_t	Disp_Bank[DISP_LINES][DISP_COLUMNS];		//es el unico banco principal en donde se tiene una copia de lo que pasa en display para hacer operaciones mas rapidas que leyendo la info del display....
//---------------------------------------------------------------------
unsigned char** Read_Disp_Bank(void)
{
	return (uint8_t**)Disp_Bank;
}
void Write_Disp_Instr(unsigned char Instr)
{
	GPIOE->PCOR = 1<<1;	//Disp_DI_Clr();
	GPIOA->PDOR=(GPIOA->PDOR&0xFFFF00FF)|(Instr<<8);
	GPIOC->PCOR = 1<<7;	//Disp_WR_Clr();
	GPIOC->PSOR = 1<<7;	//Disp_WR_Set();
	GPIOE->PSOR = 1<<1;	//Disp_DI_Set();	//ya lo dejo en modo data.. que es lo que mas se usa..
}
void Write_Disp_Data(unsigned char Data)
{
	GPIOA->PDOR=(GPIOA->PDOR&0xFFFF00FF)|(Data<<8);
	GPIOC->PCOR = 1<<7;	//Disp_WR_Clr();
	GPIOC->PSOR = 1<<7;	//Disp_WR_Set();
}
//----------------------------------------------------------------------
void Write_Disp_Bank		(void)					//14621=730useg por fila.
{
	unsigned char i;
	for(i=0;i<DISP_COLUMNS;i++) Write_Disp_Data(Disp_Bank[0][i]);
	for(i=0;i<DISP_COLUMNS;i++) Write_Disp_Data(Disp_Bank[2][i]);
	for(i=0;i<DISP_COLUMNS;i++) Write_Disp_Data(Disp_Bank[1][i]);
	for(i=0;i<DISP_COLUMNS;i++) Write_Disp_Data(Disp_Bank[3][i]);
}
void Send_Disp_Bank2Serial		(void)	
{
}
//----------------------------------------------------------------------
void Clear_Bank		(void)								
{
 uint8_t i;
 for(i=0;i<DISP_LINES;i++)
   String_Copy((uint8_t*)Clear_Data,Disp_Bank[i],DISP_COLUMNS);
}

void Init_Lcd_Pins(void)
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
	GPIO_PinInit_As_Out	(GPIOA,  8,0);			//
	GPIO_PinInit_As_Out	(GPIOA,  9,0);			//
	GPIO_PinInit_As_Out	(GPIOA, 10,0);			//
	GPIO_PinInit_As_Out	(GPIOA, 11,0);			//
	GPIO_PinInit_As_Out	(GPIOA, 12,0);			//
	GPIO_PinInit_As_Out	(GPIOA, 13,0);			//
	GPIO_PinInit_As_Out	(GPIOA, 14,0);			//
	GPIO_PinInit_As_Out	(GPIOA, 15,0);			//
}

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


unsigned char Disp_Data_Read(void)	
{
	return (GPIO_PortRead(GPIOA)&0x0000FF00)>>8;
}
void Disp_Data_Port2Out(void)
{
	GPIO_Port_As_Out(GPIOA,0x0000FF00);
}
void Disp_Data_Port2In(void)
{
	GPIO_Port_As_In(GPIOA,0x0000FF00);
}
//----------------------------------------------------------------------
void Init_Display_Phisical(void)
{
        Init_Lcd_Pins();
	Disp_CS_Clr();
	Disp_RD_Set();
	Disp_WR_Clr();
	Disp_Rst_Clr();
        Delay_Useg(1000);
	Disp_Rst_Set();			//esto es porque podria filtrarse la senial de reset sino.
        Delay_Useg(120000);		//hay que esperar 120mseg.. ver pag 230 del controlador

	Write_Disp_Instr(0x0011);	//exit SLEEP mode  /arranca en sleep in..
        Delay_Useg(5000);		//esperar 5mseg

	Write_Disp_Instr(0x00CB);	//Power Control A 
	Write_Disp_Data(0x0039);	//always 0x39
	Write_Disp_Data(0x002C);	//always 0x2C 
	Write_Disp_Data(0x0000);	//always 0x00 
	Write_Disp_Data(0x0034);	//Vcore = 1.6V 
	Write_Disp_Data(0x0002);	//DDVDH = 5.6V 

	Write_Disp_Instr(0x00CF);	//Power Control B 
	Write_Disp_Data(0x0000);	//always 0x00 
	Write_Disp_Data(0x0081);	//PCEQ off 
	Write_Disp_Data(0x0030);	//ESD protection 

	Write_Disp_Instr(0x00E8);	//Driver timing control A 
	Write_Disp_Data(0x0085);	//non‐overlap 
	Write_Disp_Data(0x0001);	//EQ timing 
	Write_Disp_Data(0x0079);	//Pre‐charge timing 

	Write_Disp_Instr(0x00EA);	//Driver timing control B 
	Write_Disp_Data(0x0000);	//Gate driver timing 
	Write_Disp_Data(0x0000);	//always 0x00 

	Write_Disp_Instr(0x00ED);	//Power‐On sequence control 
	Write_Disp_Data(0x0064);	//soft start 
	Write_Disp_Data(0x0003);	//power on sequence 
	Write_Disp_Data(0x0012);	//power on sequence 
	Write_Disp_Data(0x0081);	//DDVDH enhance on 

	Write_Disp_Instr(0x00F7);	//Pump ratio control 
	Write_Disp_Data(0x0020);	//DDVDH=2xVCI 
	 
	Write_Disp_Instr(0x00C0);	//power control 1 
	Write_Disp_Data(0x0026);
	 
	Write_Disp_Instr(0x00C1);	//power control 2 
	Write_Disp_Data(0x0011);
	 
	Write_Disp_Instr(0x00C5);	//VCOM control 1 
	Write_Disp_Data(0x0035);
	Write_Disp_Data(0x003E);

	Write_Disp_Instr(0x00C7);	//VCOM control 2 
	Write_Disp_Data(0x00BE); 

	Write_Disp_Instr(0x0036);	//memory access control = BGR 
	Write_Disp_Data(0x0088);

	Write_Disp_Instr(0x00B1);	//frame rate control 
	Write_Disp_Data(0x0000);
	Write_Disp_Data(0x0010);

	Write_Disp_Instr(0x00B6);	//display function control 
	Write_Disp_Data(0x000A);
	Write_Disp_Data(0x00A2);

	Write_Disp_Instr(0x003A);	//pixel format = 16 bit per pixel 
	Write_Disp_Data(0x0055);

	Write_Disp_Instr(0x00F2);	//3G Gamma control 
	Write_Disp_Data(0x0002);	//off 

	Write_Disp_Instr(0x0026);	//Gamma curve 3 
	Write_Disp_Data(0x0001);
 
	Write_Disp_Instr(0x002A);	//column address set 
	Write_Disp_Data(0x0000);
	Write_Disp_Data(0);	//start 0x0000 
	Write_Disp_Data(0x0000);
	Write_Disp_Data(239);	//end 0x00EF 
	 
	Write_Disp_Instr(0x002B);	//page address set 
	Write_Disp_Data(0x0000);
	Write_Disp_Data(0x0000);	//start 0x0000 
	Write_Disp_Data(0x0001);
	Write_Disp_Data(0x003F);	//end 0x013F 

	Write_Disp_Instr(0x0029);	//display On 
	Write_Disp_Data(0x0000);	//start 0x0000 


	Write_Disp_Instr(0x2C);	
	for(int j=0;j<322;j++) 
			for(int i=0;i<240;i++) {
				Write_Disp_Data(0);
				Write_Disp_Data(0);	
			}
}

unsigned char Img [] RODATA  =
{
//	#include "img.txt"
//	#include "img2.txt"
//	#include "yellow.txt"
 	 #include "calc3.txt"
};

void Write_Next(void)
{
	static int j=0;
	if(j==0)	
		Write_Disp_Instr(0x2C);	
	for(int i=0;i<240;i++) {
		Write_Disp_Data(Img[(j*240+i)*2]);
		Write_Disp_Data(Img[(j*240+i)*2+1]);
	}
	if(++j>=(sizeof(Img)/480)) j=0;
}
//----------------------------------------------------------------------

