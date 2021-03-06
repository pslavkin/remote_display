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
#include "ftm.h"
#include "dma.h"
#include "tpanel.h"
#include "adc.h"
#include "tpanel.h"
#include "flash.h"
#include "ftm.h"
#include "one_wire.h"

State
   Free_State1 [ ],
   Free_State2 [ ],
   Free_State3 [ ],
   Free_State4 [ ],
   Free_State5 [ ],
   Free_State6 [ ],
   Free_State7 [ ],
   Free_State8 [ ],
   Free_State9 [ ],
   Free_State10[ ];

State* Everythings_Sm;           //variable que lleva cuenta del estado de la maquina de estados de "detodo un poco"...
bool Ack_State;
bool Initial_Ack_Pin_State;
//-------------------------------------------------------------------
void Init_Ack_Pin(void)
{
   CLOCK_EnableClock  ( kCLOCK_PortE               );
   PORT_SetPinMux     ( PORTE ,14 ,kPORT_MuxAsGpio );
   PORT_Pullup_Enable ( PORTE ,14                  );
   GPIO_PinInit_As_In ( GPIOE ,14                  );
}
bool Read_Ack_Pin(void)
{
   return  GPIO_PinRead(GPIOE,14);
}
bool Read_Ack_State(void)
{
   return Ack_State;
}
void Reset_Ack_State(void)
{
   Initial_Ack_Pin_State=Read_Ack_Pin(); // en el primer acceso, el pin esta en
                                         // cero. para los proxumos, se
                                         // invierte
   Send_NVData2Serial    ( 19,Initial_Ack_Pin_State?"Initial Ack pin=1\r\n":"Initial Ack pin=0\r\n");
   Ack_State=false;
}
void Update_Ack_State(void)
{
   if(Read_Ack_Pin()==!Initial_Ack_Pin_State)
      Ack_State=true;
}
//----------------------------------------------------------------------------------------------------
void     Init_Everythings  (void)
{
   Everythings_Sm=Free_State1    ;
   Init_Schedule              ( );
   Init_Leds_Session          ( );
   Init_Ftm3C0                ( ); // LCD write strobe
   Init_Ftm0C0                ( ); // bligth
   Init_Display_Phisical_7789 ( );
   Init_Display_Layers        ( );
   Init_Dma                   ( );
   Init_Adc                   ( );
   Init_Tpanel                ( );
   Init_Ack_Pin               ( );
   Init_Flash                 ( );
   Init_One_Wire              ( );
}
State**  Everythings     ( void ) { return &Everythings_Sm             ;} // devuelve la direccion de la maquina de estados Everythings para poder mandarle mensajes.
void     Everythings_Rti ( void ) { Send_Event(ANY_Event,Everythings());} // manda mensajes ANY a tiempos predefinidos...
//----------------------------------------------------------------------------------------------------
State Free_State1 [ ]RODATA  =
{
{ ANY_Event ,Rien                ,Free_State2} ,
} ;
State Free_State2 [ ]RODATA  =
{ {
 ANY_Event  ,Rien                ,Free_State3  ,
}};
State Free_State3 [ ]RODATA  =
{ {
 ANY_Event  ,Update_Ack_State    ,Free_State4  ,
}};
State Free_State4 [ ]RODATA  =
{ {
 ANY_Event  ,Rien                ,Free_State5  ,
}};
State Free_State5 [ ]RODATA  =
{ {
 ANY_Event  ,Accelerate_All_Ftm0 ,Free_State6  ,
}};
State Free_State6 [ ]RODATA  =
{ {
 ANY_Event  ,Schedule            ,Free_State7  ,
}};
State Free_State7 [ ]RODATA  =
{ {
 ANY_Event  ,Rien                ,Free_State8  ,
}};
State Free_State8 [ ]RODATA  =
{ {
 ANY_Event  ,Led_Effects_Func    ,Free_State9  ,
}};
State Free_State9 [ ]RODATA  =
{ {
 ANY_Event  ,Rien                ,Free_State10 ,
}};
State Free_State10[ ]RODATA  =
{ {
 ANY_Event  ,Rien                ,Free_State1  ,
}};
//-------------------------------------------------------------------------------
