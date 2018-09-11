#ifndef  ONE_WIR
#define  ONE_WIRE

#include "state_machine.h"
#include "events.h"
//-----------------------------------------------------------

typedef struct {
   uint8_t Crc;
   uint8_t Code[6];
   uint8_t Family;
} One_Wire_Struct;


enum ONE_WIRE_TIMINGS{
   PRESENCE_TIME = 800,
   ZERO_TIME     = 120,
   ONE_TIME      =  20
};

enum One_Wire_Event_Code{
            Overflow_Event     = 0x1200 ,
            Fall_Event         = 0x1201 ,
            Zero_Event         = 0x1202 ,
            One_Event          = 0x1203 ,
            Presense_Event     = 0x1204 ,
            Error_Event        = 0x1205 ,
            Time_Invalid_Event = 0x1206 ,
            Command_End_Event  = 0x1207
};
enum ONE_WIRE_COMMANDS
{
 READ_ROM   = 0x33,
 SKIP_ROM   = 0xCC,
 MATCH_ROM  = 0x55,
 SEARCH_ROM = 0xF0
};
//-----------------------------------------------------------
extern void       One_Wire_Rti      ( void                        );
extern void       Init_One_Wire     ( void                        );
extern State**    One_Wire          ( void                        );
extern void       Receive_Command   ( void                        );
extern void       Inc_Actual_Bit    ( uint8_t Max ,uint16_t Event );
extern void       Add_One           ( void                        );
extern void       Add_Zero          ( void                        );
extern void       Print_Command     ( void                        );
extern void       Print_Presence    ( void                        );
extern void       Wait_Fall         ( void                        );
extern void       Print_Actual_Code ( void                        );
// ----------------------------------------------------
#endif

