#ifndef SERIAL_SESSION
#define SERIAl_SESSION

#include "state_machine.h"
//-----------------------------------------------------
enum Session_Event_Code {
         Data_Saved_Event = 0x0000,
         Time_Out_Event   = 0x0001
         };
//-----------------------------------------------------
extern void       Init_Serial_Session ( void );
extern State**    Serial_Session      ( void );
//-----------------------------------------------------
#endif
