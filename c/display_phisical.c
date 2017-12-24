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
 Wait_Disp_Ready(); 
 Disp_Data_Port2Out();
 Disp_RW_Clr();
 Disp_DI_Clr();
 Disp_E_Set();
 Disp_Data_Write(Instr);
 Disp_E_Clr();
}
void Wait_Disp_Ready(void)
{
 uint32_t TOut;
 Disp_Data_Port2In();
 Disp_RW_Set();
 Disp_DI_Clr();
 for(Disp_E_Set(),TOut=0;Disp_Data_Read()&0x80 && ++TOut<1000000;Disp_E_Set()) 
	Disp_E_Clr();
 Disp_E_Clr();
}
void Write_Ddram(unsigned char Data)
{
 Wait_Disp_Ready();
 Disp_Data_Port2Out();
 Disp_RW_Clr();
 Disp_DI_Set();
 Disp_E_Set();
 Disp_Data_Write(Data);
 Disp_E_Clr();
}
//----------------------------------------------------------------------
void Set_Ddram_Pos		(unsigned char Pos)			{Write_Disp_Instr(SET_DDRAM_CODE|Pos);}				//Posiciona el display en la columna actual...
void Set_Cgram_Pos		(unsigned char Pos,unsigned char Line)	{Write_Disp_Instr(SET_CGRAM_CODE|(Pos<<3)|Line);}				//Posiciona el display en la columna actual...
void Cursor_On_Off		(unsigned char On_Off)			{Write_Disp_Instr(CURSOR_OFF_CODE|(On_Off<<1));}				//Posiciona el display en la columna actual...
//----------------------------------------------------------------------
void Write_Custom_Bank		(unsigned char Length,unsigned char* Bank) 	
{
	unsigned char i;
	for (i=0,Length/=8;i<Length;i++) 
		Write_Custom_Char(i,Bank+i*8);
}
void Write_Custom_Char		(unsigned char Pos,unsigned char* Pic)			//14621=730useg por fila.
{
 unsigned char i;
 Set_Cgram_Pos(Pos,0);
 for(i=0;i<8;i++) Write_Ddram(Pic[i]);
}

void Write_Disp_Bank		(void)					//14621=730useg por fila.
{
	unsigned char i;
	Set_Ddram_Pos(0x00);
	for(i=0;i<DISP_COLUMNS;i++) Write_Ddram(Disp_Bank[0][i]);
	for(i=0;i<DISP_COLUMNS;i++) Write_Ddram(Disp_Bank[2][i]);
	for(i=0;i<DISP_COLUMNS;i++) Write_Ddram(Disp_Bank[1][i]);
	for(i=0;i<DISP_COLUMNS;i++) Write_Ddram(Disp_Bank[3][i]);
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

	//power pin PTE1
    	PORT_SetPinMux		(PORTE, 1, kPORT_MuxAsGpio);	//power
//	PORT_SetPinDriveStrength(PORTE, 1, 1);			//lo pongo como salida pulenta
	GPIO_PinInit_As_Out	(GPIOE, 1,1);			//como salida y arranca en 1

	//Contraste level 1
    	PORT_SetPinMux		(PORTE,14, kPORT_MuxAsGpio);	//
	GPIO_PinInit_As_Out	(GPIOE,14,0);			//en cero para armar el pulldown
	//Contraste level 2
    	PORT_SetPinMux		(PORTB, 6, kPORT_MuxAsGpio);	//
	GPIO_PinInit_As_Out	(GPIOB, 6,0);			//en cero para armar el pulldown
	//Contraste level 3
    	PORT_SetPinMux		(PORTB, 7, kPORT_MuxAsGpio);	//
	GPIO_PinInit_As_Out	(GPIOB, 7,0);			//en cero para armar el pulldown

	//D/I
    	PORT_SetPinMux		(PORTC, 7, kPORT_MuxAsGpio);	//
	GPIO_PinInit_As_Out	(GPIOC, 7,0);			//

	//R/W
    	PORT_SetPinMux		(PORTA,16, kPORT_MuxAsGpio);	//
	GPIO_PinInit_As_Out	(GPIOA,16,0);			//

	//E1
    	PORT_SetPinMux		(PORTE, 6, kPORT_MuxAsGpio);	//
	GPIO_PinInit_As_Out	(GPIOE, 6,0);			//
	
	//BLIGHT1
    	PORT_SetPinMux		(PORTA, 0, kPORT_MuxAsGpio);	//
	GPIO_PinInit_As_Out	(GPIOA, 0,1);			//

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
	GPIO_PinInit_As_Out	(GPIOA, 16,0);			//
}

void Disp_DI_Set	(void)			{GPIO_PortSet	(GPIOC,1<<7);}
void Disp_DI_Clr	(void)			{GPIO_PortClear	(GPIOC,1<<7);}
void Disp_E_Set		(void)			{Delay_Useg(1);	GPIO_PortSet	(GPIOE,1<<6);Delay_Useg(1);}
void Disp_E_Clr		(void)			{Delay_Useg(1);	GPIO_PortClear	(GPIOE,1<<6);Delay_Useg(1);}
void Disp_RW_Set	(void)			{GPIO_PortSet	(GPIOA,1<<16);}
void Disp_RW_Clr	(void)			{GPIO_PortClear	(GPIOA,1<<16);}

unsigned char Disp_Data_Read(void)	
{
	return (GPIO_PortRead(GPIOA)&0x0000FF00)>>8;
}
void Disp_Data_Write	(unsigned char Data)	
{
	GPIO_PortSet	(GPIOA,  Data <<8);
	GPIO_PortClear	(GPIOA,(~Data)<<8);
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
        Delay_Useg(20000);
        Disp_RW_Clr();
	Disp_DI_Clr();
        Disp_E_Set();
        Disp_Data_Write(FUN_SET_CODE);
        Disp_E_Clr();                                //los cambios se producen cuando E pasa de 1 a cero.
        Delay_Useg(5000);
        Disp_E_Set();
        Disp_Data_Write(FUN_SET_CODE);
        Disp_E_Clr();                                //los cambios se producen cuando E pasa de 1 a cero.
        Delay_Useg(200);
        Disp_E_Set();
        Disp_Data_Write(FUN_SET_CODE);
        Disp_E_Clr();                                //los cambios se producen cuando E pasa de 1 a cero.
        Delay_Useg(200);
        Disp_E_Set();
        Disp_Data_Write(FUN_SET_CODE);
        Disp_E_Clr();                                //los cambios se producen cuando E pasa de 1 a cero.
      	Delay_Useg(200);
        Disp_E_Set();
        Disp_Data_Write(DISP_ON_CODE);
        Disp_E_Clr();
	Delay_Useg(200);
        Disp_E_Set();
        Disp_Data_Write(CLEAR_CODE);
        Disp_E_Clr();
        Delay_Useg(200);
        Disp_E_Set();
        Disp_Data_Write(SHIFT_MODE_CODE);
        Disp_E_Clr();
        Delay_Useg(200);
}
//----------------------------------------------------------------------

