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

State
   Presence     [ ],
   Command      [ ],
   Parse_Command[ ];

State* One_Wire_Sm;
uint8_t Actual_Bit;
uint8_t Actual_Command;
One_Wire_Struct OW;
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

unsigned char Calc_One_Wire_Crc(unsigned char* Data,unsigned char Length)
{
 unsigned char Crc=0;
 while(Length--) Crc=Rotate_crc(Data[Length],Crc);
 return Crc;
}
void Set_OW_Code_Nibble(uint8_t Number,uint8_t Pos)
{
   OW.Code[Pos/2]&=~(Pos%2?0x0F:0xF0);
   OW.Code[Pos/2]|=Number<<(Pos%2?0:4);
}

void Set_OW_Code(uint8_t* Code)
{
   uint8_t i;
   for(i=0;i<12;i++)
      Set_OW_Code_Nibble(Code[i],i);
}
void Set_OW_Family(uint8_t Family)
{
   OW.Family=Family;
}
void Set_OW_Crc(void)
{
   uint8_t Crc=0,i;
   Crc=Rotate_crc(OW.Family,Crc);
   for(i=0;i<6;i++)
      Crc=Rotate_crc(OW.Code[5-i],Crc);
   OW.Crc=Crc;
}

void Init_One_Wire_Pin(void)
{
   CLOCK_EnableClock   ( kCLOCK_Ftm2              );
   CLOCK_EnableClock   ( kCLOCK_PortD             );
   PORT_Pullup_Enable  ( PORTD ,14                );
   PORT_SetPinMux      ( PORTD ,14 ,kPORT_MuxAlt2 ); // PTD14 como alt2 es FTM2CH5
   GPIO_PinInit_As_Out ( GPIOD ,14 ,0             );
   FTM2->CONTROLS[5].CnSC = 0x0000004A;            // capture on fall and reset CNT y genera IRQ
   FTM2->MOD              = 9375;                 //
   FTM2->SC               = 0x0000000F;            // prescaler x 128, 1.06 useg x clk (120M/128/9375=10mseg (con 0.5m me alcanzaba)
   NVIC_EnableIRQ(FTM2_IRQn); //la iRQ salta cuando termina de transferir todo el pic y recien ahi agpgo el ftm y aviso con un evento
}

void FTM2_IRQHandler(void)
{
//   if(FTM2->SC&0x00000100) {
//      FTM2->SC&= ~0x00000200;
//      Send_Event(Overflow_Event,&One_Wire_Sm);
//   }
   uint16_t Time=FTM2->CONTROLS[5].CnV;
   switch(FTM2->CONTROLS[5].CnSC&0x0000008C) {
      case 0x88:
         FTM2->CONTROLS[5].CnSC = 0x00000046;             // cambio a rise y limpio flag
         Send_Event(Fall_Event,&One_Wire_Sm);
         break;

      case 0x84: {
         FTM2->CONTROLS[5].CnSC = 0x00000000;             // apago.. ya tengo el tiempo.. vere que hacer en SM
         uint16_t Event=Time_Invalid_Event;
              if(Time<ONE_TIME)
               Event=One_Event;
         else if (Time<ZERO_TIME)
               Event=Zero_Event;
         else if (Time<PRESENCE_TIME)
               Event=Presense_Event;
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
   One_Wire_Sm=Presence;
   Init_One_Wire_Pin ( );
   Set_OW_Code(Zero_Code);
   Set_OW_Family(1);
   Set_OW_Crc();
}
State**  One_Wire     ( void ) { return &One_Wire_Sm                    ;} // devuelve la direccion de la maquina de estados One_Wire para poder mandarle mensajes.
void     One_Wire_Rti ( void ) { Atomic_Send_Event(ANY_Event,One_Wire());} // manda mensajes ANY a tiempos predefinidos...
//----------------------------------------------------------------------------------------------------
void Print_Error    ( void ){
   Send_NVData2Serial ( 7,"Error\r\n" );
//   Send_Int_NLine2Serial ( FTM2->CONTROLS[5].CnV );
   Wait_Fall();
}
void Print_Overflow ( void ){ Send_NVData2Serial(10,"Overflow\r\n");}
void Print_Fall     ( void ){ Send_NVData2Serial( 6,"Fall\r\n")    ;}

void Wait_Fall(void)
{
   FTM2->CONTROLS[5].CnSC = 0x0000004A;
}

void Print_Zero     ( void ){
   Wait_Fall();
   Send_NVData2Serial( 6,"Zero\r\n")    ;
}
void Print_One     ( void ){
   Wait_Fall();
   Send_NVData2Serial( 5,"One\r\n")    ;
}

void Wait2Print_Presence(void)
{
   New_None_Periodic_Func_Schedule(50,Print_Presence);
}

void Print_Presence     ( void ){
   Wait_Fall();
   Send_NVData2Serial(10,"Presence\r\n")    ;
//   Send_Int_NLine2Serial(FTM2->CONTROLS[5].CnV);
}
void Ack_Presence     ( void ) {
   Delay_Useg(30);
   PORT_SetPinMux     ( PORTD ,14 ,kPORT_MuxAsGpio );
   Delay_Useg(200);
   PORT_SetPinMux     ( PORTD ,14 ,kPORT_MuxAlt2 );
   Wait_Fall();
   Send_NVData2Serial ( 14,"Presence Ack\r\n" );
}

void Receive_Command(void)
{
   Actual_Bit     = 0;
   Actual_Command = 0;
}
void Inc_Actual_Bit(uint8_t Max,uint16_t Event)
{
   if(++Actual_Bit>=Max)
      Atomic_Send_Event(Event,Actual_Sm());
}
void Add_One(void)
{
   Print_One();
   Actual_Command|=(1<<Actual_Bit);
   Inc_Actual_Bit(8,Command_End_Event);
}
void Add_Zero(void)
{
   Print_Zero();
   Actual_Command&=~(1<<Actual_Bit);
   Inc_Actual_Bit(8,Command_End_Event);
}
void Print_Command(void)
{
   Send_NVData2Serial        ( 9,"Command=:"             ) ;
   Send_Hex_Int_NLine2Serial ( Actual_Command            ) ;
   Atomic_Send_Event         ( Actual_Command,Actual_Sm( ));
}
void Print_Read_Rom(void)
{
   Send_NVData2Serial(12,"Read Rom\r\n");
}
void Print_Actual_Code(void)
{
   char B[18];
   Char2Hex_Bcd   ( B     ,OW.Crc    );
   String2Hex_Bcd ( B+2 ,(char*      )&OW.Code ,6);
   Char2Hex_Bcd   ( B+14  ,OW.Family );
   B[16]='\r';
   B[17]='\n';
   Send_NVData2Serial(12,"Actual Code=");
   Send_VData2Serial(18,B);
}
//----------------------------------------------------------------------------------------------------
void Ack_Presence_And_Receive_Command(void) {Ack_Presence();Receive_Command();}
//----------------------------------------------------------------------------------------------------
State Presence [ ] RODATA  =
{
{Fall_Event     ,Print_Fall                       ,Presence} ,
{Presense_Event ,Ack_Presence_And_Receive_Command ,Command}  ,
{ANY_Event      ,Print_Error                      ,Presence} ,
};
State Command [ ] RODATA  =
{
{Fall_Event        ,Print_Fall     ,Command}       ,
{One_Event         ,Add_One        ,Command}       ,
{Zero_Event        ,Add_Zero       ,Command}       ,
{Command_End_Event ,Print_Command  ,Parse_Command} ,
{Presense_Event    ,Print_Presence ,Command}       ,
{ANY_Event         ,Print_Error    ,Command}       ,
};
State Parse_Command [ ] RODATA  =
{
{Fall_Event     ,Print_Fall          ,Parse_Command} ,
{READ_ROM       ,Print_Read_Rom      ,Parse_Command} ,
{One_Event      ,Print_One           ,Parse_Command} ,
{Zero_Event     ,Print_Zero          ,Parse_Command} ,
{Presense_Event ,Wait2Print_Presence ,Presence}      ,
{ANY_Event      ,Print_Error         ,Parse_Command} ,
};
//-------------------------------------------------------------------------------
