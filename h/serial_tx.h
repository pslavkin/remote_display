#ifndef SERIAL_TX
#define SERIAL_TX
#include "state_machine.h"

//-----------------------------------------------------------------------
#define SERIAL_TX_BUFFER   50    //considero el maximo la lista de codigos de una lista de 10 codigos cada uno de 17 bytes
//-----------------------------------------------------------------------
enum Serial_Tx_Events {
            Byte_Ready_Event            = 0x00A0,
            Message_Ready_Event         = 0x00A1,
            End_Of_Messages_Event       = 0x00A2,
            Last_Byte_Sended_Event      = 0x00A3,
            Transmit_Data_Message_Event = 0x00A4,  //solo bytes.. porque se usa en fifo como byte no como int..
            Transmit_Complete_Event     = 0x00A5
};
extern unsigned char    Sync_Remote_Event_Const;
extern unsigned char    Im_Remote_Event_Const;
//--------------------------------------------------------------------
extern void       Init_Serial_Tx ( void );
extern State**    Serial_Tx      ( void );
//--------------------------------------------------------------------
extern unsigned int*    Serial_Tx_As_PInt  ( unsigned char Pos );
extern unsigned char*   Serial_Tx_As_PChar ( unsigned char Pos );
extern unsigned char    Serial_Tx_As_Char  ( unsigned char Pos );
//--------------------------------------------------------------------
extern void       Send_NVData2Serial_Ans  (unsigned int Length,char *Data);
extern void       Send_VData2Serial_Ans   (unsigned int Length,char *Data);
//-------------------------------------------------------------------
extern void       Send_NVData2Serial                 ( unsigned int Length,char *Data                         );
extern void       Send_NVData2Serial_Until_Delimiter ( unsigned int Length,char *Data,unsigned char Delimiter );
extern void       Send_NVData2Serial_Until_Enter     ( unsigned int Length,char *Data                         );
extern void       Send_NLine_NVData2Serial           ( unsigned int Length,char *Data                         );
extern void       Send_VData2Serial                  ( unsigned int Length,char *Data                         );
extern void       Send_NLine_VData2Serial            ( unsigned int Length,char *Data                         );
// --------------------------------------------------------------------
extern void       Send_NLine2Serial         ( void                );
extern void       Send_Bool2Serial          ( unsigned char Bool  );
extern void       Send_Char2Serial          ( unsigned char Value );
extern void       Send_Char_NLine2Serial    ( unsigned char Value );
extern void       Send_Int2Serial           ( unsigned int Value  );
extern void       Send_Int_NLine2Serial     ( unsigned int Value  );
extern void       Send_Long2Serial          ( unsigned long Value );
extern void       Send_Long_NLine2Serial    ( unsigned long Value );
extern void       Send_Hex_Int_NLine2Serial ( unsigned int Value  );
// --------------------------------------------------------------------
extern void          Set_Master                  ( void                 );
extern void          Unset_Master                ( void                 );
extern void          Save_Master                 ( unsigned char Value  );
extern unsigned char Read_Master                 ( void                 );
extern void          Save_Tandem                 ( unsigned char Value  );
extern unsigned char Read_Tandem                 ( void                 );
extern void          Set_Tandem                  ( unsigned char Value  );
extern void          Set_Tandem2Local            ( void                 );
extern void          Set_Tandem2Remote           ( void                 );
extern void          Set_Tandem2Both             ( void                 );
extern void          Toogle_Tandem               ( void                 );
extern void          Send_NVEvent_Handler2Serial ( unsigned char* Event );
// --------------------------------------------------------------------
#endif

