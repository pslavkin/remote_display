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
#include "type_conversion.h"
#include "estim_phisical.h"

State   
        Free_State1[],
	Free_State2[],
	Free_State3[],
	Free_State4[],
	Free_State5[],
	Free_State6[],
	Free_State7[],
	Free_State8[],
	Free_State9[],
	Free_State10[];

State* Everythings_Sm;				//variable que lleva cuenta del estado de la maquina de estados de "detodo un poco"...
//----------------------------------------------------------------------------------------------------
void 		Init_Everythings	(void) 			
{ 
	Everythings_Sm=Free_State1;
	Init_Schedule();
	Init_Leds_Session();
	Init_Display_Layers();
	Init_Estim_Phisical();
}	
State** 	Everythings		(void) 			{return &Everythings_Sm;} 			//devuelve la direccion de la maquina de estados Everythings para poder mandarle mensajes.
void 		Everythings_Rti		(void)			{Send_Event(ANY_Event,Everythings());}		//manda mensajes ANY a tiempos predefinidos...
//----------------------------------------------------------------------------------------------------
State Free_State1 [] RODATA  =
{
{ANY_Event			,Rien 					,Free_State2},
};
State Free_State2[] RODATA  =
{{
 ANY_Event			,Rien					,Free_State3,
}};
State Free_State3[] RODATA  =
{{
 ANY_Event			,Rien					,Free_State4,
}};
State Free_State4[] RODATA  =
{{
 ANY_Event			,Rien 					,Free_State5,
}};
State Free_State5[] RODATA  =
{{
 ANY_Event			,Rien 					,Free_State6,
}};
State Free_State6[] RODATA  =
{{
 ANY_Event			,Schedule	 			,Free_State7,
}};
State Free_State7[] RODATA  =
{{
 ANY_Event			,Rien					,Free_State8,
}};
State Free_State8[] RODATA  =
{{
 ANY_Event			,Led_Effects_Func			,Free_State9,
}};
State Free_State9[] RODATA  =
{{
 ANY_Event			,Rien		 			,Free_State10,
}};
State Free_State10[] RODATA  =
{{
 ANY_Event			,Rien					,Free_State1,
}};
//-------------------------------------------------------------------------------
