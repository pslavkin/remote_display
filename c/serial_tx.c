#include "mke18f16.h"
#include "serial_phisical.h"
#include "state_machine.h"
#include "events.h"
#include "serial_rx.h"
#include "serial_tx.h"
#include "serial_tx_fifo.h"
#include "str.h"
#include "everythings.h"
#include "type_conversion.h"

static State
   Idle        [],
   Transmiting    [],
   Waiting_Last_Byte [];
//--------------------------------------------------------------------------
State* Serial_Tx_Sm;             
static unsigned char       Tx_Buffer[SERIAL_TX_BUFFER];  
struct Serial_Tx_Message   Actual_Print_Message;
//--------------------------------------------------------------------------
State** Serial_Tx (void)      {return &Serial_Tx_Sm;}
void  Init_Serial_Tx (void)
{
 Serial_Tx_Sm = Idle;
 Init_Serial_Tx_Fifo();
}
//------------------------------------------------------------------------------
unsigned int*  Serial_Tx_As_PInt (unsigned char Pos)  {return (unsigned int*) (Tx_Buffer+Pos);}
unsigned char*    Serial_Tx_As_PChar   (unsigned char Pos)  {return (unsigned char*) (Tx_Buffer+Pos);}
unsigned char     Serial_Tx_As_Char (unsigned char Pos)  {return *Serial_Tx_As_PChar(Pos);}
//------------------------------------------------------------------------------
void Search_Next  (void)      {Atomic_Send_Event(Read_Message_Type(),Serial_Tx());}
void Load_Next    (void)      {Actual_Print_Message=Atomic_Read_Serial_Tx_Message();}
void Serial_Print (void)            //rutina que imprime n bytes...
{
 if(Actual_Print_Message.Size)            //si aun quedan bytes para imprimir...
  {
   Actual_Print_Message.Size--;           //se decrementa el numero de bytes que quedan..
   Serial_Phisical_Tx(*Actual_Print_Message.Start++); //se imprime el correspondiente byte a la vez que se incrementa el puntero en uno...
  }
 else 
   Atomic_Send_Event(Message_Ready_Event,Serial_Tx());//en el caso de que haya terminado de transmitir envia el mensaje..
}
void Ans2Application (void)      {Atomic_Send_Event(Transmit_Complete_Event,Actual_Print_Message.Sm);}
//------------------------------------------------------------------------------
void Send_NV2Serial_Ans (unsigned int Length,unsigned char *Data,unsigned char Type,State** Sm) //manda datos NO VOLATILES por el serial
{
 Atomic_Add_Serial_Tx_Message(Data,Length,Type,Sm);
 Search_Next();
}
void Send_V2Serial_Ans(unsigned int Length,unsigned char *Data,unsigned char Type,State** Sm)   //manda datos NO VOLATILES por el serial
{
 String_Copy(Data,Serial_Tx_As_PChar(0),Length);
 Send_NV2Serial_Ans(Length,Serial_Tx_As_PChar(0),Type,Sm);
}
void Send_NVData2Serial_Ans   (unsigned int Length,unsigned char *Data) {Send_NV2Serial_Ans(Length,Data,Transmit_Data_Message_Event,Actual_Sm());}
void Send_VData2Serial_Ans (unsigned int Length,unsigned char *Data) {Send_V2Serial_Ans(Length,Data,Transmit_Data_Message_Event,Actual_Sm());}
//------------------------------------------------------------------------------
void Send_NLine2Serial                  ( void                                                            ) { Send_NVData2Serial(2,(unsigned char*)"\r\n")                                                                       ;}
void Send_NVData2Serial                 ( unsigned int Length,unsigned char *Data                         ) { Send_NV2Serial_Ans(Length,Data,Transmit_Data_Message_Event,Empty_State_Machine)                                    ;}
void Send_NVData2Serial_Until_Delimiter ( unsigned int Length,unsigned char *Data,unsigned char Delimiter ) { Send_NV2Serial_Ans(Find_Char_On_String(Data,Delimiter,Length),Data,Transmit_Data_Message_Event,Empty_State_Machine);}
void Send_NVData2Serial_Until_Enter     ( unsigned int Length,unsigned char *Data                         ) { Send_NVData2Serial_Until_Delimiter(Length,Data,'\r')                                                               ;}
void Send_NLine_NVData2Serial           ( unsigned int Length,unsigned char *Data                         ) { Send_NLine2Serial()                                                                                                ;Send_NVData2Serial(Length,Data);}
void Send_VData2Serial                  ( unsigned int Length,unsigned char *Data                         ) { Send_V2Serial_Ans(Length,Data,Transmit_Data_Message_Event,Empty_State_Machine)                                     ;}
void Send_NLine_VData2Serial            ( unsigned int Length,unsigned char *Data                         ) { Send_NLine2Serial()                                                                                                ;Send_VData2Serial(Length,Data) ;}
void Send_Bool2Serial                   ( unsigned char Bool                                              ) { Send_NVData2Serial(5,Bool?(unsigned char*)"On \r\n":(unsigned char*)"Off\r\n")                                     ;}
void Send_Char2Serial                   ( unsigned char Value                                             ) { Send_NVData2Serial(3,Char2Bcd(Serial_Tx_As_PChar(0),Value))                                                        ;}
void Send_Char_NLine2Serial             ( unsigned char Value                                             ) { Send_NVData2Serial(3,Char2Bcd(Serial_Tx_As_PChar(0),Value))                                                        ;Send_NLine2Serial()            ;}
void Send_Int2Serial                    ( unsigned int Value                                              ) { Send_NVData2Serial(5,Int2Bcd(Serial_Tx_As_PChar(3),Value))                                                         ;}
void Send_Int_NLine2Serial              ( unsigned int Value                                              ) { Send_NVData2Serial(5,Int2Bcd(Serial_Tx_As_PChar(3),Value))                                                         ;Send_NLine2Serial()            ;}
void Send_Hex_Int_NLine2Serial          ( unsigned int Value                                              ) { Send_NVData2Serial(4,Int2Hex_Bcd(Serial_Tx_As_PChar(3),Value))                                                     ;Send_NLine2Serial()            ;}
void Send_Long2Serial                   ( unsigned long Value                                             ) { Send_NVData2Serial(8,Long2Bcd(Serial_Tx_As_PChar(8),Value))                                                        ;}
void Send_Long_NLine2Serial             ( unsigned long Value                                             ) { Send_NVData2Serial(8,Long2Bcd(Serial_Tx_As_PChar(8),Value))                                                        ;Send_NLine2Serial()            ;}
//----------------------------------------------------------------------
void Load_Next_And_Serial_Print     (void)      {Load_Next();Serial_Print();}
void Ans2Application_And_Wait_Last_Byte   (void)      {Ans2Application();Serial_Phisical_Wait_Last_Byte();}
//------------------------------------------------------------------------------
static State Idle[] RODATA=
{
{  Transmit_Data_Message_Event      ,Load_Next_And_Serial_Print         ,Transmiting},
{  ANY_Event         ,Rien                ,Idle},
};
static State Transmiting[] RODATA=
{
{  Byte_Ready_Event     ,Serial_Print              ,Transmiting},
{  Message_Ready_Event     ,Ans2Application_And_Wait_Last_Byte    ,Waiting_Last_Byte},
{  ANY_Event         ,Rien                ,Transmiting},
};
static State Waiting_Last_Byte[] RODATA=
{
{  Byte_Ready_Event     ,Search_Next               ,Idle},
{  ANY_Event            ,Rien                                     ,Waiting_Last_Byte},
};
//------------------------------------------------------------------------------

