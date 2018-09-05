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
#include "mask_pic.h"

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

State**  Tpanel     ( void ) { return &TPanel_Sm               ;}
void     Tpanel_Rti ( void ) { Send_Event(ANY_Event,&TPanel_Sm);}
//--------------------------------------------------------------
void     Print_TPanel_Raw  (void)
{
   unsigned char *T=Serial_Tx_As_PChar(0);
   Int2Bcd  ( T+0  ,Tp.X        );
   Int2Bcd  ( T+6  ,Tp.Y        );
   Int2Bcd  ( T+12 ,Tp.X_Scaled );
   Int2Bcd  ( T+18 ,Tp.Y_Scaled );
   Char2Bcd ( T+24 ,Tp.Touched  );
   T[ 5  ]=' ';
   T[ 11 ]=' ';
   T[ 17 ]=' ';
   T[ 23 ]=' ';
   T[ 27 ]=' ';
   T[ 28 ]='\r';
   T[ 29 ]='\n';
   Send_NVData2Serial(30,T);
}
//--------------------------------------------------------------
void Test_Touched      (void)
{
  Tp.Touched=( Tp.Touched<<1 |                           // con esto me acuerdo de donde vengo
             ((Tp.X<=TPanel_Limits.Max_X &&              // busco a ver si estoy apretando
               Tp.X>=TPanel_Limits.Min_X &&
               Tp.Y<=TPanel_Limits.Max_Y &&
               Tp.Y>=TPanel_Limits.Min_Y)?0x01:0x00)) &
               0x03;                                     // solo me quedo con 2 bits
  Atomic_Send_Event(Tp.Touched,Actual_Sm());
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
 Delay_Before_Drag=DELAY_BEFORE_DRAG_FILTER                  ;
 Find_Event_Handler ( None_Button,Tp.X_Scaled,Tp.Y_Scaled,0 );
}
void On_Drag         (void)            //escala y busca un handler
{
 if(Delay_Before_Drag) Delay_Before_Drag--;
  else
  {
   Scale_X ( );
   Scale_Y ( );
   Find_Event_Handler ( None_Button ,Tp.X_Scaled ,Tp.Y_Scaled ,1 );
  }
}
void On_Release         (void)
{
   struct Struct_Pic_Pos P= { 0 ,0 ,0 ,0};
   Find_Event_Handler(None_Button,Tp.X_Scaled,Tp.Y_Scaled,2);
   Set_Mask_Pic             ( &P );
   Layer_Structure_Modified (    );
}
//--------------------------------------------------------------------
void Init_Tpanel(void)
{
   TPanel_Sm  = Initializing;
   Tp.Touched = 0;
   CLOCK_EnableClock  ( kCLOCK_PortA                         );

   PORT_SetPinMux           ( PORTA ,0  ,kPORT_MuxAsGpio ); // YU Pull up
   PORT_SetPinDriveStrength ( PORTA ,0  ,1               );
   GPIO_PinInit_As_In       ( GPIOA ,0                   );
   PORT_Pullup_Enable       ( PORTA ,0                   );

   PORT_SetPinMux           ( PORTA ,15 ,kPORT_MuxAsGpio ); // YD ADC12
   PORT_SetPinDriveStrength ( PORTA ,15 ,1               );
   GPIO_PinInit_As_Out      ( GPIOA ,15 ,1               ); // YD 5v

   PORT_SetPinMux           ( PORTA ,1  ,kPORT_MuxAsGpio ); // XR Pull up
   PORT_SetPinDriveStrength ( PORTA ,1  ,1               );
   PORT_Pullup_Enable       ( PORTA ,1                   );
   GPIO_PinInit_As_In       ( GPIOA ,1                   );

   PORT_SetPinMux           ( PORTA ,16 ,kPORT_MuxAsGpio ); // XL ADC13
   PORT_SetPinDriveStrength ( PORTA ,16 ,1               );
   GPIO_PinInit_As_Out      ( GPIOA ,16 ,0               ); // XL 0v
}

void Feed_X_And_Free_Y(void)
{
   //Send_NVData2Serial(16,(unsigned char*)"Feed x free Y \r\n");

   GPIO_PinInit_As_Out ( GPIOA ,1  ,1                         ); // XR pullup -> 5v
   PORT_SetPinMux      ( PORTA ,16 ,kPORT_MuxAsGpio           ); // XL ADC13  -> 0v

   GPIO_PinInit_As_In  ( GPIOA ,0                             ); // YU 0v -> pullup
   PORT_SetPinMux      ( PORTA ,15 ,kPORT_PinDisabledOrAnalog ); // YD 5v -> ADC12
}
void Feed_Y_And_Free_X(void)
{
   //Send_NVData2Serial(16,(unsigned char*) "Feed y free x \r\n");

   GPIO_PinInit_As_Out ( GPIOA ,0  ,0                         ); // YU pullup -> 0v
   PORT_SetPinMux      ( PORTA ,15 ,kPORT_MuxAsGpio           ); // YD ADC12  -> 5v

   GPIO_PinInit_As_In  ( GPIOA ,1                             ); // XR 5v -> pullup
   PORT_SetPinMux      ( PORTA ,16 ,kPORT_PinDisabledOrAnalog ); // XL 5v -> ADC13
}
void Read_X ( void ) { Tp.X= Read_Adc(12);} //cuando hago feed de X, leo en el ADC del Y, la coordenada X
void Read_Y ( void ) { Tp.Y= Read_Adc(13);} //cuando hago feed de Y, leo en el ADC del X, la coordenada Y
//--------------------------------------------------------------------
void Read_X_And_Feed_Y_And_Free_X                  ( void ) { Read_X();Feed_Y_And_Free_X();}
void Read_Y_And_Feed_X_And_Free_Y_And_Test_Touched ( void ) { Read_Y();Feed_X_And_Free_Y();Test_Touched();}
//----------------------------------------------------
State Initializing   [ ]=
{
{ ANY_Event          ,Feed_X_And_Free_Y                             ,Reading_Y       },
};
State Reading_Y      [ ]=
{
{ ANY_Event          ,Read_X_And_Feed_Y_And_Free_X                  ,Reading_X       },
};
State Reading_X      [ ]=
{
{ ANY_Event          ,Read_Y_And_Feed_X_And_Free_Y_And_Test_Touched ,Testing_Touched },
};
State Testing_Touched[ ]=
{
{ None_Touched_Event ,Rien                                          ,Reading_Y       },
{ Click_Event        ,On_Click                                      ,Reading_Y       },
{ Drag_Event         ,On_Drag                                       ,Reading_Y       },
{ Released_Event     ,On_Release                                    ,Reading_Y       },
{ ANY_Event          ,Rien                                          ,Testing_Touched },
};
//----------------------------------------------------
