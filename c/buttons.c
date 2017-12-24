#include <stdio.h>
#include "board.h"
#include "mke18f16.h"
#include "fsl_lpuart.h"
#include "fsl_port.h"
#include "everythings.h"
#include "state_machine.h"
#include "events.h"
#include "serial_tx.h"
#include "schedule.h"
#include "leds_session.h"
#include "buttons.h"
#include "debug.h"
#include "display_phisical.h"
#include "display_layers.h"
#include "rotary.h"
#include "type_conversion.h"
#include "buzzer.h"
#include "power_pic.h"

State 
	Button_Disabled		[],
	Button_Any_Pressed	[],
	Button_None_Pressed	[],
	Button_Rotary_Pressed	[],
	Button_Rotary_Hold1	[],
	Button_Rotary_Hold2	[];

unsigned char Accel;
State* Buttons_Sm;
//-------------------------------------------------------------------------------------
void 		Init_Buttons		(void) 	
{
	Buttons_Sm=Button_Any_Pressed;
	INIT_BUTTONS();
}
State** 	Buttons			(void)	{return &Buttons_Sm;}
void		Buttons_Rti		(void)	{Atomic_Send_Event(READ_ALL_BUTTONS(),Buttons());}
//--------------------------------------------------------------------------------------
void 		Reset_Accel		(void)	{Accel=0;}
void 		Release_Hold_Button	(void)	{Atomic_Send_Event(Wait_Any_Button_Release_Event,Actual_Sm());}
//--------------------------------------------------------------------------------------
void 	Button_Rotary_Press		(void)	
{
	Send_NVDebug_Buttons_Data2Serial(22,"Button Rotary Pressed\n");	
	Find_Event_Handler(Button_Rotary_Code,0);
	Reset_Accel();
}
void 	Button_Rotary_Released	(void)	
{
	Send_NVDebug_Buttons_Data2Serial(19,"Button Rotary Released\n");		
	if(Find_Event_Handler(Button_Rotary_Code,2)) Beep();
}
void 	Button_Rotary_Hold		(void)	
{
	Send_NVDebug_Buttons_Data2Serial(19,"Button Rotary Hold\n");		
	if(++Accel>HOLD_DELAY) Atomic_Send_Event(Button_Rotary_Hold_Event,Actual_Sm());
}
void 	Button_Rotary_Next_Hold		(void)
{
	Beep();
	Reset_Accel();
}
void 	Button_Rotary_Hold1_Released	(void)	
{
	Send_NVDebug_Buttons_Data2Serial(19,"Button Rotary Hold1\n");		
	if(Find_Event_Handler(Button_Rotary_Code,1)) Beep();
}
void 	Button_Rotary_Hold2_Released	(void)	
{
	Send_NVDebug_Buttons_Data2Serial(19,"Button Rotary Hold2\n");
	Del_All_Layers_But_Power();
	Beep();
}
void 	Button_Rotary_Hold3_Released	(void)	
{
	Send_NVDebug_Buttons_Data2Serial(19,"Button Rotary Hold3\n");
	Del_All_Layers_But_Power();
	Beep();
}
//--------------------------------------------------------------------------------------
State Button_Disabled [] RODATA =
{
{ ANY_Event			,Rien				,Button_Disabled},
};
//--------------------------------------------------------------------------------------
State Button_Any_Pressed [] RODATA =
{
{ Button_None_Event		,Rien				,Button_None_Pressed},
{ ANY_Event			,Rien				,Button_Any_Pressed},
};
//-------------------------------------------------------
State Button_None_Pressed [] RODATA =
{
{ Button_Rotary_Event		,Button_Rotary_Press		,Button_Rotary_Pressed},
{ ANY_Event			,Rien				,Button_None_Pressed},
};
//---------ROTARY----------------------------------------
State Button_Rotary_Pressed [] RODATA =
{
{ Button_None_Event		,Button_Rotary_Released		,Button_None_Pressed},
{ Button_Rotary_Event		,Button_Rotary_Hold		,Button_Rotary_Pressed}, 
{ Button_Rotary_Hold_Event	,Button_Rotary_Next_Hold	,Button_Rotary_Hold1}, 
{ ANY_Event			,Rien				,Button_Rotary_Pressed},
};
 State Button_Rotary_Hold1 [] RODATA =
{
{ Button_None_Event		,Button_Rotary_Hold1_Released	,Button_None_Pressed},
{ Button_Rotary_Event		,Button_Rotary_Hold		,Button_Rotary_Hold1}, 
{ Button_Rotary_Hold_Event	,Button_Rotary_Next_Hold	,Button_Rotary_Hold2}, 
{ ANY_Event			,Rien				,Button_Rotary_Hold1},
};
State Button_Rotary_Hold2 [] RODATA =
{
{ Button_None_Event		,Button_Rotary_Hold2_Released	,Button_None_Pressed},
{ Button_Rotary_Event		,Button_Rotary_Hold		,Button_Rotary_Hold2}, 
{ Button_Rotary_Hold_Event	,Button_Rotary_Hold3_Released	,Button_Any_Pressed}, 
{ ANY_Event			,Rien				,Button_Rotary_Hold2},
};
//--------------------------------------------------------------------------------------

