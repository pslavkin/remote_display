#include "mke18f16.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "everythings.h"
#include "state_machine.h"
#include "events.h"
#include "schedule.h"
#include "leds_session.h"
#include "type_conversion.h"

static  State
   Off[];

State *Leds_Session_Sm;
//-------------------------------------------------------------------
void Init_Leds(void)
{
   CLOCK_EnableClock   ( kCLOCK_PortE               );
   PORT_SetPinMux      ( PORTE ,5U ,kPORT_MuxAsGpio );
   GPIO_PinInit_As_Out ( GPIOE ,5U ,0               );
}
void Led_Run_On  ( void ) { GPIO_PortClear ( GPIOE,1<<5);}
void Led_Run_Off ( void ) { GPIO_PortSet   ( GPIOE,1<<5);}
void Fan_On      ( void ) { }
void Fan_Off     ( void ) { }
//-------------------------------------------------------------------
struct Led_Effect_Struct Led_Effects[]=
{
 {0x8000 ,0xFFAA ,Led_Run_On ,Led_Run_Off} ,
// {0xAAAA ,0x00AA ,Rien       ,Rien}        ,
};
void Set_Led_Effect        ( unsigned char Led,unsigned int Effect ) { Led_Effects[Led].Effect=Led_Effects[Led].Temp_Effect=Effect                                     ;}
void Set_Temp_Led_Effect   ( unsigned char Led,unsigned int Effect ) { Led_Effects[Led].Temp_Effect=Effect                                                             ;}
void Set_Toogle_Led_Effect ( unsigned char Led                     ) { Led_Effects[Led].Temp_Effect&0x01?Led_Effects[Led].Off_Function():Led_Effects[Led].On_Function();}
void Set_Fixed_Led_Effect  ( unsigned char Led,unsigned int Effect ) { Led_Effects[Led].Effect=Effect                                                                  ;}
void Led_Effects_Func      (void)
{
 unsigned char Actual_Led;
 for(Actual_Led=0;Actual_Led<sizeof(Led_Effects)/sizeof(struct Led_Effect_Struct);Actual_Led++)
  {
   (Led_Effects[Actual_Led].Temp_Effect&0x0001)?Led_Effects[Actual_Led].On_Function():Led_Effects[Actual_Led].Off_Function(); //se usa el metodo del shifteop para trasladar la indo de la variable al estado del led...
   if(!(Led_Effects[Actual_Led].Temp_Effect>>=1)) Led_Effects[Actual_Led].Temp_Effect=Led_Effects[Actual_Led].Effect;
  }
}
//------------------------------------------------------------------
void Init_Leds_Session(void)
{
   Leds_Session_Sm=Off;
   Init_Leds();
}
State**  Leds_Session      (void)      {return &Leds_Session_Sm;}
void     Leds_Session_Rti  (void)      {Led_Effects_Func();}
//-------------------------------------------------------------------
static State Off[] RODATA =
{
 {ANY_Event    ,Rien    ,Off},
};
//-------------------------------------------------------------------------------
