#include <stdio.h>
#include "board.h"
#include "mke18f16.h"
#include "fsl_lpuart.h"
#include "fsl_port.h"
#include "everythings.h"
#include "state_machine.h"
#include "events.h"
#include "serial_tx.h"
#include "schedule.h"
#include "leds_session.h"
#include "debug.h"
#include "display_phisical.h"
#include "display_layers.h"
#include "type_conversion.h"
#include "buzzer.h"
#include "adc.h"
#include "tpanel.h"

State
   Initializing   [ ],
   Reading_Y      [ ],
   Reading_X      [ ],
   Testing_Touched[ ];

struct TPanel_Limits_Struct TPanel_Limits= {
      TP_MAX_Y,
      TP_MIN_Y,
      TP_MAX_X,
      TP_MIN_X,
      TP_MAX_Y-TP_MIN_Y,
      TP_MAX_X-TP_MIN_X
   };

State*               TPanel_Sm        ;//variable que lleva cuenta del estado de la maquina de estados de "detodo un poco"...
unsigned char        Delay_Before_Drag;
struct TPanel_Struct Tp               ;

State**  Tpanel    ( void ) { return &TPanel_Sm           ;}
void     Tpanel_Rti( void ) { Atomic_Send_Event(Tp.Touched,&TPanel_Sm);}
//--------------------------------------------------------------
void     Print_TPanel_Raw  (void)
{
   unsigned char *T=Serial_Tx_As_PChar(0);
   Char2Bcd(T+0 ,Tp.X);
   Char2Bcd(T+4 ,Tp.Y);
   Char2Bcd(T+8 ,Tp.X_Scaled);
   Char2Bcd(T+12,Tp.Y_Scaled);
   Char2Bcd(T+16,Tp.Touched);
   T[3 ]=' ';
   T[7 ]=' ';
   T[11]=' ';
   T[15]=' ';
   T[19]=' ';
   T[20]='\r';
   T[21]='\n';
   Send_NVData2Serial(22,T);
}
//--------------------------------------------------------------
void Test_Touched      (void)
{
  Tp.Touched=Tp.Touched << 1 |
             (Tp.X<=TPanel_Limits.Max_X && Tp.X>=TPanel_Limits.Min_X && Tp.Y<=TPanel_Limits.Max_Y && Tp.Y>=TPanel_Limits.Min_Y)?0x01:0x00;
}
unsigned char TPanel_Scaled_X( void ) { return Tp.X_Scaled;}
unsigned char TPanel_Scaled_Y( void ) { return Tp.Y_Scaled;}
unsigned char TPanel_X       ( void ) { return Tp.X       ;}
unsigned char TPanel_Y       ( void ) { return Tp.Y       ;}
//--------------------------------------------------------------------
void Scale_X(void)
{
 Tp.X_Scaled=( Tp.X>TPanel_Limits.Min_X )?(Tp.X-TPanel_Limits.Min_X):0 ;//corre el offset teniendo en cuenta que no de negativo, por si eventualmente llega un valor menor que el minimo...
 Tp.X_Scaled=( Tp.X_Scaled*239)/TPanel_Limits.Delta_X;
}
void Scale_Y(void)
{
 Tp.Y_Scaled=( Tp.Y>TPanel_Limits.Min_Y )?(Tp.Y-TPanel_Limits.Min_Y):0  ;
 Tp.Y_Scaled=( Tp.Y_Scaled*319)/TPanel_Limits.Delta_Y;
}
//------------------------
void On_Click        (void)            //escala y busca algun handler
{
 Scale_X ( );
 Scale_Y ( );
 Delay_Before_Drag=DELAY_BEFORE_DRAG_FILTER                    ;
 Find_Event_Handler(Invalid_Button,Tp.X_Scaled,Tp.Y_Scaled,0 );
}
void On_Drag         (void)            //escala y busca un handler
{
 if(Delay_Before_Drag) Delay_Before_Drag--;
  else
  {
   Scale_X ( );
   Scale_Y ( );
   Find_Event_Handler ( Invalid_Button ,Tp.X_Scaled ,Tp.Y_Scaled ,1 );
  }
}
void On_Release         (void)
{
   if(Find_Event_Handler(Invalid_Button,Tp.X_Scaled,Tp.Y_Scaled,2)) {
      }
   Set_Mask_Pic ( 0 ,0 ,0 ,0 );
}
//--------------------------------------------------------------------
void Init_Tpanel(void)
{
   TPanel_Sm  = Initializing;
   Tp.Touched = 0;
   CLOCK_EnableClock  ( kCLOCK_PortA                         );

   PORT_SetPinMux           ( PORTA ,0  ,kPORT_MuxAsGpio ); // YU Pull up
   PORT_SetPinDriveStrength ( PORTA ,0  ,1               );
   GPIO_PinWrite            ( GPIOA ,0  ,0               ); // YU 0v
   GPIO_Port_As_In          ( GPIOA ,0                   );
   PORT_Pullup_Enable       ( PORTA ,0                   );

   PORT_SetPinMux           ( PORTA ,15 ,kPORT_MuxAsGpio ); // YD ADC12
   PORT_SetPinDriveStrength ( PORTA ,15 ,1               );
   GPIO_PinWrite            ( GPIOA ,15 ,1               ); // YD 5v
   GPIO_Port_As_Out         ( GPIOA ,15                  );

   PORT_SetPinMux           ( PORTA ,1  ,kPORT_MuxAsGpio ); // XR Pull up
   PORT_SetPinDriveStrength ( PORTA ,1  ,1               );
   GPIO_PinWrite            ( GPIOA ,1  ,0               ); // XR 0v
   PORT_Pullup_Enable       ( PORTA ,1                   );
   GPIO_Port_As_In          ( GPIOA ,1                   );

   PORT_SetPinMux           ( PORTA ,16 ,kPORT_MuxAsGpio ); // XL ADC13
   PORT_SetPinDriveStrength ( PORTA ,16 ,1               );
   GPIO_PinWrite            ( GPIOA ,16 ,1               ); // XL 5v
   GPIO_Port_As_Out         ( GPIOA ,16                  );
}

void Feed_X_And_Free_Y(void)
{
   GPIO_Port_As_Out ( GPIOA ,1                             ); // XR pullup -> 0v
   PORT_SetPinMux   ( PORTA ,16 ,kPORT_MuxAsGpio           ); // XL ADC13  -> 5v

   GPIO_Port_As_In  ( GPIOA ,0                             ); // YD 0v -> pullup
   PORT_SetPinMux   ( PORTA ,15 ,kPORT_PinDisabledOrAnalog ); // YU 5v -> ADC12
}
void Feed_Y_And_Free_X(void)
{
   GPIO_Port_As_Out ( GPIOA ,0                             ); // YU pullup -> 0v
   PORT_SetPinMux   ( PORTA ,15 ,kPORT_MuxAsGpio           ); // YD ADC12  -> 5v

   GPIO_Port_As_In  ( GPIOA ,1                             ); // XR 0v -> pullup
   PORT_SetPinMux   ( PORTA ,16 ,kPORT_PinDisabledOrAnalog ); // XL 5v -> ADC13
}
void Read_X ( void ) { Tp.X= Read_Adc(13);}
void Read_Y ( void ) { Tp.Y= Read_Adc(12);}
//--------------------------------------------------------------------
void Read_Y_And_Feed_Y_And_Free_X                  ( void ) { Read_Y();Feed_Y_And_Free_X();}
void Read_X_And_Test_Touched_And_Feed_X_And_Free_Y ( void ) { Read_X();Test_Touched();Feed_X_And_Free_Y();}
//----------------------------------------------------
State Initializing   [ ]=
{
{ ANY_Event          ,Feed_X_And_Free_Y                             ,Reading_Y       } ,
};
State Reading_Y      [ ]=
{
{ ANY_Event          ,Read_Y_And_Feed_Y_And_Free_X                  ,Reading_X       } ,
};
State Reading_X      [ ]=
{
{ ANY_Event          ,Read_X_And_Test_Touched_And_Feed_X_And_Free_Y ,Testing_Touched } ,
};
State Testing_Touched[ ]=
{
{ None_Touched_Event ,Read_Y_And_Feed_Y_And_Free_X                  ,Reading_X       } ,
{ Click_Event        ,On_Click                                      ,Reading_Y       } ,
{ Drag_Event         ,On_Drag                                       ,Reading_Y       } ,
{ Released_Event     ,On_Release                                    ,Reading_Y       } ,
{ ANY_Event          ,Read_Y_And_Feed_Y_And_Free_X                  ,Reading_X       } ,
};
//----------------------------------------------------
