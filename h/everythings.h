//archivo h para el manejo de los leds y el buzzer
#ifndef  EVERYTHINGS
#define  EVERYTHINGS

#include "state_machine.h"
#include <stdbool.h>

//-----------------------------------------------------------
enum Everythings_Event_Code{
            Everythings_Null_Event  = 0
                 };
//-----------------------------------------------------------
extern void       Everythings_Rti  ( void );
extern void       Init_Everythings ( void );
extern State**    Everythings      ( void );
// ----------------------------------------------------
extern bool       Read_Ack_Pin     ( void );
extern void       Init_Ack_Pin     ( void );
extern void       Update_Ack_State ( void );
extern void       Reset_Ack_State  ( void );
bool              Read_Ack_State   ( void );
// ----------------------------------------------------
#endif

