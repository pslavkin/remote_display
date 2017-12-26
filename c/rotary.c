#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "mke18f16.h"
#include "fsl_lpuart.h"
#include "fsl_port.h"
#include "everythings.h"
#include "state_machine.h"
#include "events.h"
#include "serial_tx.h"
#include "schedule.h"
#include "leds_session.h"
#include "display_phisical.h"
#include "display_layers.h"
#include <rti.h>
#include <rotary.h>
#include <debug.h>

State 
	CW		[],
	ACW		[],
	CW_Handler	[],
	ACW_Handler	[];
	
State*			Rotary_Sm;
struct Struct_Rotary 	Rotary_Params;
//unsigned char	Rotary_ACW_Code_Const 		=Rotary_ACW_Code;
//unsigned char	Rotary_CW_Code_Const  		=Rotary_CW_Code;	
//-------------------------------------------------------------------------------------
void 		Init_Rotary		(void) 	
{
// PORT_PTAPE_PTAPE3	=1; 	//pullup A3
// PORT_PTEPE_PTEPE6	=1; 	//pullup E6
// PORT_PTEIE_PTEIE6	=1;	//hablito entrada para leer el pin E6, el A3 sale por irq.....
// KBI0_PE_KBIPE3		=1; 	//kbi pin enable, un solo pin basta...so el A3. SOLO si soy master...
// KBI0_SC		=0x06;	//irq enable y borra alguna irq pendiente...
 Rotary_Params.Count	=0;
 Rotary_Params.Accel	=1;
 Rotary_Sm		=CW;
 New_Periodic_Schedule(ROTARY_TOUT,Reset_Rotary_Count_Event,Rotary());
}
State** 	Rotary			(void)	{return &Rotary_Sm;}
void		Rotary_Rti		(void)	{Atomic_Send_Event(ANY_Event,Rotary());}
//--------------------------------------------------------------------------------------
unsigned char 	Read_Rotary_Filter	(void)			{return ROTARY_FILTER;}
unsigned char 	Read_Rotary_TOut	(void)			{return ROTARY_TOUT;}
unsigned char 	Read_Rotary_Accel	(void)			{return Rotary_Params.Accel;}
void	 	Print_Rotary_Accel	(void)			{Send_Char_NLine2Serial(Rotary_Params.Accel);}
void 		Print_Rotary_Sm_Add	(void)			{Send_Hex_Int_NLine2Serial((unsigned int)Rotary_Sm);}
//--------------------------------------------------------------------------------------
void Rotary_Irq(void)
{
//	KBI0_SC_KBACK=1;	
//	Rotary_Params.Actual_Tick=Rti_Tick;	
//	Send_Event(PORT_PTED&PORT_PTED_PTED6_MASK,Rotary());
}
void Reset_Rotary_Count		(void)	
{
 Send_NVDebug_Rotary_Data2Serial(11,"Rotary Rst\n");
 Rotary_Params.Count=0;
 Rotary_Params.Accel=1;
}
static void Calculate_Accel(void)
{
	unsigned long Accel;
	Atomic(Accel=Rotary_Params.Actual_Tick-Rotary_Params.Last_Tick);
	if(Accel<MAX_ACCEL) {
		if(Rotary_Params.Accel<15) Rotary_Params.Accel++;	
	}
	else 	Rotary_Params.Accel=1;
	Rotary_Params.Repetition=Rotary_Params.Accel*Rotary_Params.Accel;
	Rotary_Params.Last_Tick=Rotary_Params.Actual_Tick;	
}

static void Handler_CW(void)	
{
	if(Rotary_Params.Repetition)  {
		//Find_Event_Handler(Rotary_CW_Code,0); 
		Rotary_Params.Repetition--;	
	}
	Atomic_Send_Event(Rotary_Params.Repetition?Rotary_CW_Handler_Event:Rotary_CW_Handler_End_Event,Actual_Sm()); 
}
static void Print_CW(void)	
{
	Send_NVDebug_Rotary_Data2Serial(3,"CW\n");	
	Update_Schedule(ROTARY_TOUT,Reset_Rotary_Count_Event,Rotary());
	if(Rotary_Params.Count>=ROTARY_FILTER) {
 		Rotary_Params.Count=0;
		Calculate_Accel();
		Atomic_Send_Event(Rotary_CW_Handler_Event,Actual_Sm());
	}
	else Rotary_Params.Count++;
}
static void Handler_ACW(void)	
{
	if(Rotary_Params.Repetition) {
//		Find_Event_Handler(Rotary_ACW_Code,0); 
		Rotary_Params.Repetition--;	
	}
	Atomic_Send_Event(Rotary_Params.Repetition?Rotary_ACW_Handler_Event:Rotary_ACW_Handler_End_Event,Actual_Sm()); 
}
static void Print_ACW(void)	
{	
	Send_NVDebug_Rotary_Data2Serial(4,"ACW\n");
	Update_Schedule(ROTARY_TOUT,Reset_Rotary_Count_Event,Rotary());
	if(Rotary_Params.Count<=-ROTARY_FILTER) {
 		Rotary_Params.Count=0;
		Calculate_Accel();
		Atomic_Send_Event(Rotary_ACW_Handler_Event,Actual_Sm());
	}
	else Rotary_Params.Count--;
}
//--------------------------------------------------------------------------------------
void Reset_Rotary_Count_And_Print_ACW	(void)	{Reset_Rotary_Count();Print_ACW();}
void Reset_Rotary_Count_And_Print_CW	(void)	{Reset_Rotary_Count();Print_CW();}
//--------------------------------------------------------------------------------------
State CW[] =
{
{Rotary_CW_Event		,Print_CW				,CW},
{Rotary_CW_Handler_Event	,Handler_CW				,CW_Handler},
{Rotary_ACW_Event		,Print_ACW				,ACW},
{Reset_Rotary_Count_Event	,Reset_Rotary_Count			,CW},
{ANY_Event			,Rien					,CW},
};	
State CW_Handler[] =
{
{Rotary_CW_Handler_Event	,Handler_CW				,CW_Handler},
{Rotary_CW_Handler_End_Event	,Rien					,CW},
{ANY_Event			,Rien					,CW_Handler},
};	
State ACW[] =
{
{Rotary_ACW_Event		,Print_ACW				,ACW},
{Rotary_ACW_Handler_Event	,Handler_ACW				,ACW_Handler},
{Rotary_CW_Event		,Print_CW				,CW},
{Reset_Rotary_Count_Event	,Reset_Rotary_Count			,ACW},
{ANY_Event			,Rien					,ACW},
};
State ACW_Handler[] =
{
{Rotary_ACW_Handler_Event	,Handler_ACW				,ACW_Handler},
{Rotary_ACW_Handler_End_Event	,Rien					,ACW},
{ANY_Event			,Rien					,ACW_Handler},
};	
//--------------------------------------------------------------------------------------

