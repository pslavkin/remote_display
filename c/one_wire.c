#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "mke18f16.h"
#include "fsl_lpuart.h"
#include "fsl_port.h"
#include "one_wire.h"
#include "state_machine.h"
#include "events.h"
#include "serial_tx.h"
#include "schedule.h"
#include "type_conversion.h"
#include "str.h"

State
   Idle         [ ],
   Presence     [ ],
   Command      [ ],
   Writing_Code [ ];

State* One_Wire_Sm;
bool  Busy;
uint8_t Actual_Bit;
uint8_t Actual_Command;
uint8_t OW[8];                                     //CRC|1|2|3|4|5|6|FAMILY
uint8_t Zero_Code[12]={0,0,0,0,0,0,0,0,1,2,3,4};
void (*PTimer_Irq) (void);
//-------------------------------------------------------------------
void Init_One_Wire_Pin(void)
{
   CLOCK_EnableClock   ( kCLOCK_Ftm2              );
   CLOCK_EnableClock   ( kCLOCK_PortD             );
   PORT_Pullup_Enable  ( PORTD ,14                );
   PORT_SetPinMux      ( PORTD ,14 ,kPORT_MuxAlt2 ); // PTD14 como alt2 es FTM2CH5
   GPIO_PinInit_As_Out ( GPIOD ,14 ,0             );
   FTM2->CONTROLS[5].CnSC = 0x00000000;              // apagado al arrancar.. solo lo prendo cuando tengo que mandar algo
   FTM2->MOD              = 9375;                    //
   FTM2->SC               = 0x0000000F;              // prescaler x 128, 1.06 useg x clk (120M/128/9375=10mseg (con 0.5m me alcanzaba)
   NVIC_EnableIRQ(FTM2_IRQn);                        // la iRQ salta cuando termina de transferir todo el pic y recien ahi agpgo el ftm y aviso con un evento
}
void     Init_One_Wire  (void)
{
   One_Wire_Sm=Idle;
   Init_One_Wire_Pin ( );
   Set_OW_Code(Zero_Code);
   Set_OW_Family(1);
   Set_OW_Crc();
   Clear_Busy();
}
State**  One_Wire ( void ) { return &One_Wire_Sm;}
//----------------------------------------------------------------------------------------------------
uint8_t Rotate_crc          ( uint8_t New, uint8_t Last  ) {
   uint8_t i;
   for(i=0;i<8;i++,New>>=1)
      Last=((Last^New)&0x01)?(Last>>1)^POLYVAL:Last>>1;
   return Last;
}
void Set_OW_Code(uint8_t* Code) {
   uint8_t i;
   for(i=0;i<6;i++)
      OW[CODE_POS+i]=Code[i*2]<<4 | (Code[i*2+1]&0x0F);
}
void Set_OW_Family(uint8_t Family) {
   OW[FAMILY_POS]=Family;
}
void Set_OW_Crc(void) {
   uint8_t Crc=0,i=8;
   while(--i)
      Crc=Rotate_crc(OW[i],Crc);
   OW[CRC_POS]=Crc;
}

void Write_New_Code(uint8_t* Code)
{
   Set_OW_Code(Code);
   Set_OW_Family(DEFAULT_FAMILY);
   Set_OW_Crc();
   Print_Actual_Code();
   Atomic_Send_Event(New_Code_Event,One_Wire());
}
//-------------------------------------------------------------------
void Waiting_Fall(void)
{
   FTM2->CONTROLS[5].CnSC&=~0x00000080;
   Wait_Rise();
   Send_Event(Fall_Event,&One_Wire_Sm);
}
void Waiting_Rise(void)
{
   uint16_t Event;
   uint16_t Time=FTM2->CONTROLS[5].CnV;
   FTM2->CONTROLS[5].CnSC&=~0x00000080;
      if(Time<ONE_TIME) {
         Wait_None();
         Event=One_Event;
      }
   else if (Time<ZERO_TIME) {
         Wait_Fall();
         Event=Zero_Event;
   }
   else if (Time<PRESENCE_TIME) {
         Wait_None();
         Event=Presense_Event;
   }
   else {
         Event=Time_Invalid_Event;
         Wait_None();
   }
   Send_Event(Event,&One_Wire_Sm);
}
void FTM2_IRQHandler(void) { PTimer_Irq(); }
//----------------------------------------------------------------------------------------------------
void Print_Error    ( void ) { Send_NVData2Serial( 7,"Error\r\n" )  ; }
void Print_Fall     ( void ) { }//Send_NVData2Serial( 6,"Fall\r\n")    ;}
void Print_Zero     ( void ) { }//Send_NVData2Serial( 6,"Zero\r\n" )   ;}
void Print_One      ( void ) { }//Send_NVData2Serial( 5,"One\r\n" )    ;}
void Print_Presence     ( void ) { Send_NVData2Serial(10,"Presence\r\n")    ;}
void Print_Code_Sended  ( void ) { Send_NVData2Serial(13,"Code Sended\r\n") ;}
void Print_Abort        ( void ) { Send_NVData2Serial( 7,"Abort\r\n")       ;}
void Print_Time_Invalid ( void ) { Send_NVData2Serial(14,"Time invalid\r\n");}
void Print_Command(void) {
   Send_NVData2Serial        ( 9,"Command=:"             ) ;
   Send_Hex_Int_NLine2Serial ( Actual_Command            ) ;
}
void Print_Actual_Code(void)
{
   char B[18];
   String2Hex_Bcd ( B ,(char*      )OW ,8);
   B[16]='\r';
   B[17]='\n';
   Send_NVData2Serial(12,"Actual Code=");
   Send_VData2Serial(18,B);
}
//----------------------------------------------------------------------------------------------------
void Wait_Fall(void) {
   PTimer_Irq=Waiting_Fall;
   FTM2->CONTROLS[5].CnSC = 0x0000004A;
}
void Wait_Rise(void) {
   PTimer_Irq=Waiting_Rise;
   FTM2->CONTROLS[5].CnSC = 0x00000046;
}
void Wait_None(void) {
   FTM2->CONTROLS[5].CnSC = 0x00000000;
}
//----------------------------------------------------------------------------------------------------
void Ack_Presence     ( void ) {
   Delay_Useg(30);
   PORT_SetPinMux     ( PORTD ,14 ,kPORT_MuxAsGpio );
   Delay_Useg(200);
   PORT_SetPinMux     ( PORTD ,14 ,kPORT_MuxAlt2 );
   Delay_Useg(10);
   Wait_Fall();
   Print_Presence();
}
void Write_Next_Bit(void) {
   bool Bit=Read_Bit4String(OW,Actual_Bit);
   Delay_Useg(2);
   if(Bit==0)
      PORT_SetPinMux     ( PORTD ,14 ,kPORT_MuxAsGpio );
   Delay_Useg(18);
   PORT_SetPinMux     ( PORTD ,14 ,kPORT_MuxAlt2 );
   Delay_Useg(10);
   Wait_Fall();
   if(Actual_Bit--==0)
      Atomic_Send_Event(Code_Sended_Event,One_Wire());
}
//----------------------------------------------------------------------------------------------------
void Begin_Read_Command(void) {
   Actual_Bit     = 7;
}
void Begin_Write_Code(void) {
   Actual_Bit=63;
   Print_Command();
}
void Next_Command_Bit(void)
{
   if(Actual_Bit--==0)
      Atomic_Insert_Event(Actual_Command&0x00FF,Actual_Sm());
}
void Read_One(void)
{
   Wait_Fall();
   Set_Bit_On_String(&Actual_Command,Actual_Bit);
   Next_Command_Bit();
   Print_One();
}
void Read_Zero(void)
{
   Clear_Bit_On_String(&Actual_Command,Actual_Bit);
   Next_Command_Bit();
   Print_Zero();
}
void Send_One_Wire_Abort_Event ( void ) { Atomic_Send_Event(Abort_Event,One_Wire())   ;}
void Send_New_OW_Code          ( void ) { Atomic_Send_Event(New_Code_Event,One_Wire());}
void Clear_Busy                ( void ) { Busy=false                                  ;}
void Set_Busy                  ( void ) { Busy=true                                   ;}
bool One_Wire_Busy             ( void ) { return Busy                                 ;}
//----------------------------------------------------------------------------------------------------
void Ack_Presence_And_Begin_Read_Command            ( void ) { Ack_Presence()      ;Begin_Read_Command();}
void Print_Presence_And_Wait_Fall                   ( void ) { Print_Presence()    ;Wait_Fall()         ;}
void Print_Error_And_Wait_Fall                      ( void ) { Print_Error()       ;Wait_Fall()         ;}
void Print_Zero_And_Wait_Fall                       ( void ) { Print_Zero()        ;Wait_Fall()         ;}
void Print_Code_Sended_And_Wait_None_And_Clear_Busy ( void ) { Print_Code_Sended() ;Wait_None()         ;Clear_Busy();}
void Print_Time_Invalid_And_Wait_Fall               ( void ) { Print_Time_Invalid();Wait_Fall()         ;}
void Print_Abort_And_Wait_None_And_Clear_Busy       ( void ) { Print_Abort()       ;Wait_None()         ;Clear_Busy();}
void Wait_Fall_And_Set_Busy                         ( void ) { Wait_Fall()         ;Set_Busy()          ;}
//----------------------------------------------------------------------------------------------------
State Idle        [ ]RODATA  =
{
{ New_Code_Event     ,Wait_Fall_And_Set_Busy                         ,Presence     },
{ ANY_Event          ,Print_Error                                    ,Idle         },
                                                                                   };
State Presence    [ ]RODATA  =
{
{ Fall_Event         ,Print_Fall                                     ,Presence     },
{ Presense_Event     ,Ack_Presence_And_Begin_Read_Command            ,Command      },
{ New_Code_Event     ,Rien                                           ,Presence     },
{ Time_Invalid_Event ,Print_Time_Invalid_And_Wait_Fall               ,Presence     },
{ Abort_Event        ,Print_Abort_And_Wait_None_And_Clear_Busy       ,Idle         },
{ ANY_Event          ,Print_Error_And_Wait_Fall                      ,Presence     },
                                                                                   };
State Command     [ ]RODATA  =
{
{ Fall_Event         ,Print_Fall                                     ,Command      },
{ One_Event          ,Read_One                                       ,Command      },
{ Zero_Event         ,Read_Zero                                      ,Command      },
{ READ_ROM           ,Begin_Write_Code                               ,Writing_Code },
{ New_Code_Event     ,Rien                                           ,Command      },
{ Presense_Event     ,Print_Presence_And_Wait_Fall                   ,Presence     },
{ Abort_Event        ,Print_Abort_And_Wait_None_And_Clear_Busy       ,Idle         },
{ Time_Invalid_Event ,Print_Time_Invalid_And_Wait_Fall               ,Presence     },
{ ANY_Event          ,Print_Error_And_Wait_Fall                      ,Presence     },
                                                                                   };
State Writing_Code[ ]RODATA  =
{
{ Fall_Event         ,Print_Fall                                     ,Writing_Code },
{ One_Event          ,Write_Next_Bit                                 ,Writing_Code },
{ Zero_Event         ,Print_Zero_And_Wait_Fall                       ,Presence     },
{ Code_Sended_Event  ,Print_Code_Sended_And_Wait_None_And_Clear_Busy ,Idle         },
{ New_Code_Event     ,Rien                                           ,Writing_Code },
{ Presense_Event     ,Print_Presence_And_Wait_Fall                   ,Presence     },
{ Abort_Event        ,Print_Abort_And_Wait_None_And_Clear_Busy       ,Idle         },
{ Time_Invalid_Event ,Print_Time_Invalid_And_Wait_Fall               ,Presence     },
{ ANY_Event          ,Print_Error_And_Wait_Fall                      ,Presence     },
                                                                                   };
//-------------------------------------------------------------------------------
