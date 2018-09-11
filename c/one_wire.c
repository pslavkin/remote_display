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
#include "type_conversion.h"

State
   Master_Reset [ ];

State* One_Wire_Sm;
//-------------------------------------------------------------------
void Init_One_Wire_Pin(void)
{
   CLOCK_EnableClock  ( kCLOCK_Ftm2              );
   CLOCK_EnableClock  ( kCLOCK_PortD             );
   PORT_Pullup_Enable ( PORTD ,14                );
   GPIO_PinInit_As_In ( GPIOD ,14                );
   PORT_SetPinMux     ( PORTD ,14 ,kPORT_MuxAlt2 ); // PTD14 como alt2 es FTM2CH5
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
   switch(FTM2->CONTROLS[5].CnSC&0x0000008C) {
      case 0x88:
         FTM2->CONTROLS[5].CnSC = 0x00000046;             // cambio a rise y limpio flag
         Send_Event(Fall_Event,&One_Wire_Sm);
         break;

      case 0x84: {
         uint16_t Time=FTM2->CONTROLS[5].CnV;
         uint16_t Event=Error_Event;
         FTM2->CONTROLS[5].CnSC = 0x0000000A;             // apago.. ya tengo el tiempo.. vere que hacer en SM
         if(Time<20)
               Event=One_Event;
         else if (Time<120)
               Event=Zero_Event;
         else if (Time<600)
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
   One_Wire_Sm=Master_Reset;
   Init_One_Wire_Pin ( );
}
State**  One_Wire     ( void ) { return &One_Wire_Sm                    ;} // devuelve la direccion de la maquina de estados One_Wire para poder mandarle mensajes.
void     One_Wire_Rti ( void ) { Atomic_Send_Event(ANY_Event,One_Wire());} // manda mensajes ANY a tiempos predefinidos...
//----------------------------------------------------------------------------------------------------
void Print_Error    ( void ){ 
   Send_NVData2Serial    ( 7,"Error\r\n"         );
   Send_Int_NLine2Serial ( FTM2->CONTROLS[5].CnV );
}
void Print_Overflow ( void ){ Send_NVData2Serial(10,"Overflow\r\n");}
void Print_Fall     ( void ){ Send_NVData2Serial( 6,"Fall\r\n")    ;}
void Print_Zero     ( void ){ 
   Send_NVData2Serial( 6,"Zero\r\n")    ;
   Send_Int_NLine2Serial(FTM2->CONTROLS[5].CnV);
}
void Print_One     ( void ){
   Send_NVData2Serial( 5,"One\r\n")    ;
   Send_Int_NLine2Serial(FTM2->CONTROLS[5].CnV);
}
void Print_Presence     ( void ){
   Send_NVData2Serial(10,"Presence\r\n")    ;
   Send_Int_NLine2Serial(FTM2->CONTROLS[5].CnV);
}
//----------------------------------------------------------------------------------------------------
State Master_Reset [ ] RODATA  =
{
{Fall_Event     ,Print_Fall     ,Master_Reset} ,
{One_Event      ,Print_One      ,Master_Reset} ,
{Zero_Event     ,Print_Zero     ,Master_Reset} ,
{Presense_Event ,Print_Presence ,Master_Reset} ,
{Overflow_Event ,Print_Overflow ,Master_Reset} ,
{ANY_Event      ,Print_Error    ,Master_Reset} ,
};
//-------------------------------------------------------------------------------
