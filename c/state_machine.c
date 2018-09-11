#include "state_machine.h"
#include "events.h"
#include <stdint.h>
#include "type_conversion.h"
#include "debug.h"
#include "serial_tx.h"
#include "leds_session.h"
#include "fsl_gpio.h"
#include <stdint.h>

State* ActualState;
Events Event;
State Doing_Nothing[] RODATA;
//-----------------------------------------------------------------------
void        Set_State  ( State* New_State,State** Machine ) { *Machine=New_State                 ;}
State**     Empty_Sm   ( void                             ) { return (State**)Empty_State_Machine;}
State*      Empty_App  ( void                             ) { return Doing_Nothing               ;}
void        Rien       ( void                             ) { }
void        Delay_Useg ( uint32_t Useg                    )
{
 volatile uint32_t A=Useg*17;
  while(--A)
   ;
}
void Pum(void)
{
   while(1);
}
//-----------------------------------------------------------------------
unsigned int   Actual_Event ( void ) { return Event.Event  ;}
State**        Actual_Sm    ( void ) { return Event.Machine;}
void           Soft_Reset   ( void ) {
SCB->AIRCR = 0x5FA;
}
//-----------------------------------------------------------------------
void State_Machine(void)               //esta funcion ejecuta la maquina de estados donde el evento viene en la variable Event... que se decidio que no sea por parametro para permitir la recursividad infinita...  
{
// Disable_Irq();
 Atomic(Event=Read_Event());
// Enable_Irq();
 if(Event.Machine!=Empty_State_Machine)
 {
  ActualState = *(Event.Machine);
  for(;ActualState->Event!=ANY_Event && ActualState->Event!=Event.Event;ActualState++)
   ;
  *Event.Machine=ActualState->Next_State;
  ActualState->Func();
 }
 return;
}

//------------------------------------------------------------------------------------------
void  Print_Doing_Nothing  (void)   {}
//------------------------------------------------------------------------------------------
State Doing_Nothing[] RODATA =
{{
ANY_Event      ,Print_Doing_Nothing    ,Doing_Nothing
}};
//------------------------------------------------------------------------------------------

