#ifndef EVENTS
#define EVENTS

#include <state_machine.h>
//-----------------------------------------------------------
#define MAX_EVENTS      250//40 //40//40//80 //25 //ojo que en linux se mandan paquetes largos y el parser mete todo a eventos... por ejemplo el cambio de ip's es A192.168.002.010 255.255.255.000 192.168.002.001\r\n = 50 EVENTOS!!! mas alguno que ande por alli!!...
#define MAX_EVENT_LISTENER    5

typedef struct Events 
   {
    uint32_t Event;
    State** Machine;
   }Events;
//-----------------------------------------------------------
extern void    Init_Events         ( void                               );
extern void    Atomic_Send_Event   ( unsigned int Event,State** Machine );
extern void    Send_Event          ( unsigned int Event,State** Machine );
extern void    Atomic_Insert_Event ( unsigned int Event,State** Machine );
extern void    Insert_Event        ( unsigned int Event,State** Machine );
extern Events  Read_Event          ( void                               );

//--------------EVENT LISTENER-------------------------------------------
extern void    Init_Event_Listener       ( void                                                 );
extern void    Send_Event2Event_Listener ( unsigned int Search_Event,unsigned int Sending_Event );
extern void    Add_Event_Listener        ( unsigned int Event,State** Machine                   );
extern void    Free_Event_Listener       ( unsigned int Event,State** Machine                   );
extern void    Free_All_Event_Listener   ( State** Machine                                      );
//-------------------------------------------------------------
#endif


