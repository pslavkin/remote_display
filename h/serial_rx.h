#ifndef SERIAL_RX
#define SERIAL_RX
//-------------------------------------------------------------------
#include <state_machine.h>
#define SERIAL_RX_TOUT  200               //se usar para almacentar lo que se lee de un bloque de 512 de la SD...ver como hacer para bajarlo...

enum Serial__Receiver_Events 
       {
   End_Receiving_Event     =0x0800,
   Rx_Ready_Event       =0x0801,
   Save_Data_Event         =0x0802,
   Time_Out_Receiving_Event   =0x0803,
   Resending_Event         =0x0804
   };
//--------------------------------------------------------------------
extern void       Init_Serial_Rx    (void);
extern State**    Data_Serial_Rx    (void);
//--------------------------------------------------------------------
extern void       Save4Serial                           ( unsigned char Length,char *Data,unsigned char Time_Out,unsigned char Del                );
extern void       Save4Serial2Rx_Buffer                 ( unsigned char Length,unsigned char Pos,unsigned char Time_Out                           );
extern void       Save4Serial2Rx_Buffer_Until_Delimiter ( unsigned char Length,unsigned char Pos,unsigned char Time_Out,unsigned char Delimiter   );
extern void       Save4Serial2Rx_Buffer_Until_Enter     ( unsigned char Length                                                                    );
extern void       Save4Serial_Until_Enter               ( unsigned char Length,unsigned char* Data                                                );
//--------------------------------------------------------------------
extern char              Serial_Rx_As_Char         ( unsigned int Pos );
extern char*             Serial_Rx_As_PChar        ( unsigned int Pos );
extern unsigned int      Serial_Rx_As_Int          ( unsigned int Pos );
extern unsigned int*     Serial_Rx_As_PInt         ( unsigned int Pos );
extern unsigned long     Serial_Rx_As_Long         ( unsigned int Pos );
extern unsigned long*    Serial_Rx_As_PLong        ( unsigned int Pos );
//--------------------------------------------------------------------
extern void       Save_Char4Serial  (void);
extern void       Save_Int4Serial   (void);
extern void       Save_Long4Serial  (void);
extern void       Send_Loaded_Char2Serial (void);
extern void       Send_Loaded_Int2Serial  (void);
extern void       Send_Loaded_Long2Serial (void);
//--------------------------------------------------------------------
#endif

