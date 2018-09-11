#ifndef  ONE_WIR
#define  ONE_WIRE

#include "state_machine.h"
#include "events.h"
//-----------------------------------------------------------
enum One_Wire_Event_Code{
            Overflow_Event = 0x1200,
            Fall_Event     = 0x1201,
            Zero_Event     = 0x1202,
            One_Event      = 0x1203,
            Presense_Event = 0x1204,
            Error_Event    = 0x1205 
           };
//-----------------------------------------------------------
extern void       One_Wire_Rti  ( void );
extern void       Init_One_Wire ( void );
extern State**    One_Wire      ( void );
// ----------------------------------------------------
#endif

