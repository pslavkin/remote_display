#ifndef  LEDS_SESSION
#define  LEDS_SESSION

#include "state_machine.h"
//-----------------------------------------------------------
enum Leds_Session_Event_Code{
            Leds_Session_Null_Event = 0xD000
                 };
//-----------------------------------------------------------
extern void    Init_Leds   ( void );
extern void    Led_Run_On  ( void );
extern void    Led_Run_Off ( void );
extern void    Fan_On      ( void );
extern void    Fan_Off     ( void );
//-----------------------------------------------------------
extern void       Leds_Session_Rti  ( void );
extern void       Init_Leds_Session ( void );
extern State**    Leds_Session   ( void );
//----------------------------------------------------
extern void    Set_Led_Effect        ( unsigned char Led,unsigned int Effect );
extern void    Set_Temp_Led_Effect   ( unsigned char Led,unsigned int Effect );
extern void    Set_Fixed_Led_Effect  ( unsigned char Led,unsigned int Effect );
extern void    Set_Toogle_Led_Effect ( unsigned char Led                     );
extern void    Led_Effects_Func      ( void                                  );
//---------------------------------------------------------
enum Leds_Position
{
 Led_Run         = 0,
 Led_Ch1_Index   = 1,
 Led_Ch2_Index   = 1,
 Led_Ch3_Index   = 1,
 Led_Ch4_Index   = 1,
 Led_Ch5_Index   = 1,
 Led_Ch6_Index   = 1,
 Led_Ch7_Index   = 1,
 Led_Ch8_Index   = 1,
 Led_Red_Index   = 1,
 Led_Blue_Index  = 1,
 Led_Green_Index = 1,
};

struct Led_Effect_Struct
 {
  unsigned int Effect;
  unsigned int Temp_Effect;
  void (*On_Function)(void);
  void (*Off_Function)(void);
 };
//---------------------------------------------------------
extern void Led_Run_Off    (void);
extern void Led_Run_On     (void);
#endif

