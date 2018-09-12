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
   Sending_Data [ ];

State* One_Wire_Sm;
uint8_t Actual_Bit;
uint8_t Actual_Command;
uint8_t OW[8];                                     //CRC|1|2|3|4|5|6|FAMILY
uint8_t Zero_Code[12]={0,0,0,0,0,0,0,0,1,2,3,4};
//-------------------------------------------------------------------
unsigned char  Rotate_crc          ( unsigned char New, unsigned char Last  )
{
//8-bit CRC value  using polynomial  X^8 + X^5 + X^4 + 1 
 #define POLYVAL 0x8C
 unsigned char i;
 for(i=0;i<8;i++) {
  Last=((Last^New)&0x01)?(Last>>1)^POLYVAL:Last>>1;
  New>>=1;
 }
 return Last;
}
void Set_OW_Code(uint8_t* Code)
{
   uint8_t i;
   for(i=0;i<6;i++)
      OW[CODE_POS+i]=Code[i*2]<<4 | (Code[i*2+1]&0x0F);
}
void Set_OW_Family(uint8_t Family)
{
   OW[FAMILY_POS]=Family;
}
void Set_OW_Crc(void)
{
   uint8_t Crc=0,i=8;
   while(--i)
      Crc=Rotate_crc(OW[i],Crc);
   OW[CRC_POS]=Crc;
}

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

void Waiting_Fall(void)
{
   Send_Event(Fall_Event,&One_Wire_Sm);
   FTM2->CONTROLS[5].CnSC = 0x00000046;             // cambio a rise y limpio flag
}
void Waiting_Rise(void)
{
   uint16_t Time=FTM2->CONTROLS[5].CnV;
   uint16_t Event=Time_Invalid_Event;

      if(Time<ONE_TIME) {
         Event=One_Event;
         FTM2->CONTROLS[5].CnSC = 0x00000000;             // apago.. ya tengo el tiempo.. vere que hacer en SM
      }
   else if (Time<ZERO_TIME) {
         Event=Zero_Event;
         FTM2->CONTROLS[5].CnSC = 0x0000004A;             //vuelvo a prender antes de que venga de nuevo fall.. como es zero, no tengo que intervenir el pin.. solo saber que llego cero
   }
   else if (Time<PRESENCE_TIME) {
         FTM2->CONTROLS[5].CnSC = 0x00000000;             // apago.. ya tengo el tiempo.. vere que hacer en SM
         Event=Presense_Event;
   }
   Send_Event(Event,&One_Wire_Sm);
}

void FTM2_IRQHandler(void)
{
   uint16_t Time=FTM2->CONTROLS[5].CnV;
   switch(FTM2->CONTROLS[5].CnSC&0x0000008C) {
      case 0x88:
         FTM2->CONTROLS[5].CnSC = 0x00000046;             // cambio a rise y limpio flag
         Send_Event(Fall_Event,&One_Wire_Sm);
         break;

      case 0x84: {
         uint16_t Event=Time_Invalid_Event;
            if(Time<ONE_TIME) {
               Event=One_Event;
               FTM2->CONTROLS[5].CnSC = 0x00000000;             // apago.. ya tengo el tiempo.. vere que hacer en SM
            }
         else if (Time<ZERO_TIME) {
               Event=Zero_Event;
               FTM2->CONTROLS[5].CnSC = 0x0000004A;             //vuelvo a prender antes de que venga de nuevo fall.. como es zero, no tengo que intervenir el pin.. solo saber que llego cero
         }
         else if (Time<PRESENCE_TIME) {
               FTM2->CONTROLS[5].CnSC = 0x00000000;             // apago.. ya tengo el tiempo.. vere que hacer en SM
               Event=Presense_Event;
         }
         Send_Event(Event,&One_Wire_Sm);
         }
         break;
      default:
         Send_Event(Error_Event,&One_Wire_Sm);
         break;
   }
}

bool Read_One_Wire_Pin(void)
{
   return  GPIO_PinRead(GPIOD,14);
}
//----------------------------------------------------------------------------------------------------
void     Init_One_Wire  (void)
{
   One_Wire_Sm=Idle;
   Init_One_Wire_Pin ( );
   Set_OW_Code(Zero_Code);
   Set_OW_Family(1);
   Set_OW_Crc();
}
State**  One_Wire     ( void ) { return &One_Wire_Sm                    ;} // devuelve la direccion de la maquina de estados One_Wire para poder mandarle mensajes.
void     One_Wire_Rti ( void ) { Atomic_Send_Event(ANY_Event,One_Wire());} // manda mensajes ANY a tiempos predefinidos...
//----------------------------------------------------------------------------------------------------
void Print_Error    ( void ) { Send_NVData2Serial( 7,"Error\r\n" )  ;}
void Print_Overflow ( void ) { Send_NVData2Serial(10,"Overflow\r\n");}
void Print_Fall     ( void ) { Send_NVData2Serial( 6,"Fall\r\n")    ;}

void Wait_Fall(void) { FTM2->CONTROLS[5].CnSC = 0x0000004A; }
void Wait_Rise(void) { FTM2->CONTROLS[5].CnSC = 0x00000046; }
void Wait_None(void) { FTM2->CONTROLS[5].CnSC = 0x00000000; }

void Print_Zero     ( void ){
   Send_NVData2Serial( 6,"Zero\r\n")    ;
}
void Print_One     ( void ){
   Send_NVData2Serial( 5,"One\r\n")    ;
}
void Print_Presence     ( void ){
   Send_NVData2Serial(10,"Presence\r\n")    ;
}
void Ack_Presence     ( void ) {
   Delay_Useg(30);
   PORT_SetPinMux     ( PORTD ,14 ,kPORT_MuxAsGpio );
   Delay_Useg(200);
   PORT_SetPinMux     ( PORTD ,14 ,kPORT_MuxAlt2 );
   Delay_Useg(10);
   Wait_Fall();
   Send_NVData2Serial ( 14,"Presence Ack\r\n" );
}

void Receive_Command(void)
{
   Actual_Bit     = 7;
//   Actual_Command = 0;
}
void Next_Command_Bit(void)
{
   if(Actual_Bit--==0)
      Atomic_Send_Event(Actual_Command,Actual_Sm());
}
void Add_One(void)
{
   Wait_Fall();
   Set_Bit_On_String(&Actual_Command,Actual_Bit);
   Next_Command_Bit();
   Print_One();
}
void Add_Zero(void)
{
   Clear_Bit_On_String(&Actual_Command,Actual_Bit);
   Next_Command_Bit();
   Print_Zero();
}
void Print_Command(void)
{
   Send_NVData2Serial        ( 9,"Command=:"             ) ;
   Send_Hex_Int_NLine2Serial ( Actual_Command            ) ;
}
void Print_Read_Rom(void)
{
   Send_NVData2Serial(12,"Read Rom\r\n");
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
void Begin_Write_Rom(void)
{
   Print_Command();
   Actual_Bit=63;
}
void Send_New_OW_Code(void)
{
   Atomic_Send_Event(New_Code_Event,One_Wire());
}
void Write_Next_Bit(void)
{
   bool Bit=Read_Bit4String(OW,Actual_Bit);
   Delay_Useg(2);
   if(Bit==0)
         PORT_SetPinMux     ( PORTD ,14 ,kPORT_MuxAsGpio );
   Send_NVData2Serial(1,Bit?"1":"0");
   Delay_Useg(20);
   PORT_SetPinMux     ( PORTD ,14 ,kPORT_MuxAlt2 );
   Delay_Useg(10);
   Wait_Fall();
   if(Actual_Bit--==0)
      Atomic_Send_Event(Code_Sended_Event,One_Wire());
}
void Print_Code_Sended(void)
{
   Send_NVData2Serial(10,"Code Sended\r\n");
   Wait_None();
}
//----------------------------------------------------------------------------------------------------
void Ack_Presence_And_Receive_Command(void) {Ack_Presence();Receive_Command();}
//----------------------------------------------------------------------------------------------------
State Idle         [ ]RODATA  =
{
{ New_Code_Event    ,Wait_Fall                        ,Presence}      ,
{ ANY_Event         ,Print_Error                      ,Idle}          ,
};
State Presence     [ ]RODATA  =
{
{ Fall_Event        ,Print_Fall                       ,Presence}      ,
{ Presense_Event    ,Ack_Presence_And_Receive_Command ,Command}       ,
{ ANY_Event         ,Print_Error                      ,Presence}      ,
};
State Command      [ ]RODATA  =
{
{ Fall_Event        ,Print_Fall                       ,Command}       ,
{ One_Event         ,Add_One                          ,Command}       ,
{ Zero_Event        ,Add_Zero                         ,Command}       ,
{ READ_ROM          ,Begin_Write_Rom                  ,Sending_Data}  ,
{ Presense_Event    ,Print_Presence                   ,Idle}       , //esto seria un error..vuelve a emprezr 
{ ANY_Event         ,Print_Error                      ,Idle}       ,
};
State Sending_Data [ ]RODATA  =
{
{ Fall_Event        ,Print_Fall        ,Sending_Data },
{ One_Event         ,Write_Next_Bit    ,Sending_Data },
{ Zero_Event        ,Print_Zero        ,Idle         },//no puede venier cero.
{ Code_Sended_Event ,Print_Code_Sended ,Idle         },
{ Presense_Event    ,Print_Presence    ,Idle         },//no puede venir presence
{ ANY_Event         ,Print_Error       ,Idle         },
};
//-------------------------------------------------------------------------------
