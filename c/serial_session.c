#include "serial_session.h"
#include "serial_rx.h"
#include "serial_tx.h"
#include "state_machine.h"
#include "events.h"
#include "serial_phisical.h"
#include "everythings.h"
#include "rti.h"
#include "type_conversion.h"
#include "display_layers.h"

State
	Parsing_Main[],
	Parsing_Working[],
	Parsing_Layers[];

State* Serial_Session_Sm;
//---------------------------------------------------------------------
unsigned char About_Menu[] RODATA=
{
 "NOTO N1104 DCIV5.4\r\n"
};
unsigned char Main_Menu[] RODATA=
{
 "NOTO N1104 Main menu\r\n"
 "A Working\r\n"
 "B Buzzer\r\n"
 "C Info\r\n"
 "D Layers\r\n"
 "R Reset\r\n"
 ". About\r\n"
 "? Help\r\n"
};
unsigned char Working_Menu[] RODATA=
{
 "Working menu\r\n"
 "< Back\r\n"
 "? Help\r\n"
};
unsigned char Layers_Menu[] RODATA=
{
 "Layers menu\r\n"
 "A Actual Layer\r\n"
 "B Layer Modified\r\n"
 "C Layer Used\r\n"
 "< Back\r\n"
 "? Help\r\n"
};
//
//---------------------------------------------------------------------
void Print_About_Menu			(void)	{Send_NVData2Serial(sizeof(About_Menu)-1,(unsigned char*)About_Menu);}
void Print_Main_Menu			(void)	{Send_NVData2Serial(sizeof(Main_Menu)-1,(unsigned char*)Main_Menu);}
void Print_Layers_Menu			(void)	{Send_NVData2Serial(sizeof(Layers_Menu)-1,(unsigned char*)Layers_Menu);}
void Print_Working_Menu			(void)	{Send_NVData2Serial(sizeof(Working_Menu)-1,(unsigned char*)Working_Menu);}
//--------------------------------------------------------------------
State** 	Serial_Session		(void) 	{return &Serial_Session_Sm;} 
void 		Init_Serial_Session	(void)
{
 Serial_Session_Sm = Parsing_Main;
 Init_Serial_Rx();
 Init_Serial_Tx();
}
//---------------------------------------------------------------------
State Parsing_Main[] RODATA=
{
{ 'A' 				,Rien                          		,Parsing_Working},
{ 'D' 				,Print_Layers_Menu            		,Parsing_Layers},
{ '.' 				,Print_About_Menu              		,Parsing_Main},
{ '?' 				,Print_Main_Menu			,Parsing_Main},
{ ANY_Event  			,Rien                          		,Parsing_Main},
};
State Parsing_Working[] RODATA=
{
{'<' 				,Rien					,Parsing_Main},
{'?' 				,Print_Working_Menu			,Parsing_Working},
{ ANY_Event  			,Rien                          		,Parsing_Working},
};
State Parsing_Layers[] RODATA=
{
//{'A' 				,Print_Actual_Layers			,Parsing_Layers},
//{'B' 				,Print_Layer_Modified			,Parsing_Layers},
//{'C' 				,Print_Layers_Used			,Parsing_Layers},
{'<' 				,Rien					,Parsing_Main},
{'?' 				,Print_Layers_Menu			,Parsing_Layers},
{ ANY_Event  			,Rien                          		,Parsing_Layers},
};
//------------------------------------------------------------------------------
