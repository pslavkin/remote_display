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

const struct TPanel_Limits_Struct Default_TPanel_Limits= {
      TP_MAX_Y,
      TP_MIN_Y,
      TP_MAX_X,
      TP_MIN_X,
      TP_MAX_Y-TP_MIN_Y,
      TP_MAX_X-TP_MIN_X
   };

State*               TPanel_Sm        ;//variable que lleva cuenta del estado de la maquina de estados de "detodo un poco"...
unsigned char        Index            ;
unsigned char        Delay_Before_Drag;
struct TPanel_Struct Tp               ;

State**  Tpanel    ( void ) { return &TPanel_Sm           ;}
void     Tpanel_Rti( void ) { Send_Event(Index,&TPanel_Sm);}
//--------------------------------------------------------------
void     Print_TPanel_Limits  (void)
{
//   unsigned char *T=Serial_Tx_As_PChar(0);
//   Char2Bcd(T+0 ,TPanel_Limits.Max_Y);
//   Char2Bcd(T+4 ,TPanel_Limits.Min_Y);
//   Char2Bcd(T+8 ,TPanel_Limits.Max_X);
//   Char2Bcd(T+12,TPanel_Limits.Min_X);
//   Char2Bcd(T+16,TPanel_Limits.Delta_Y);
//   Char2Bcd(T+20,TPanel_Limits.Delta_X);
//   T[3 ]=' ';
//   T[7 ]=' ';
//   T[11]=' ';
//   T[15]=' ';
//   T[19]=' ';
//   T[23]=' ';
//   T[24]='\r';
//   T[25]='\n';
//   Send_NVData2Serial(26,T);
}
void     Print_TPanel_Raw  (void)
{
   unsigned char *T=Serial_Tx_As_PChar(0);
//   Send_NVData2Serial(sizeof(struct TPanel_Struct),(unsigned char*)&Tp);
   Char2Bcd(T+0 ,Tp.X);
   Char2Bcd(T+4 ,Tp.Y);
   Char2Bcd(T+8 ,Tp.X_Scaled);
   Char2Bcd(T+12,Tp.Y_Scaled);
   Char2Bcd(T+16,Tp.Button);
   Char2Bcd(T+20,Tp.Last_Button);
   Char2Bcd(T+24,Tp.Touched);
   T[3 ]=' ';
   T[7 ]=' ';
   T[11]=' ';
   T[15]=' ';
   T[19]=' ';
   T[23]=' ';
   T[27]=' ';
   T[28]='\r';
   T[29]='\n';
   Send_NVData2Serial(30,T);
}
//--------------------------------------------------------------
void     Print_Buttons  (void)
{
}
//--------------------------------------------------------------
unsigned char  Read_TPanel_Limits    ( unsigned char Pos                     ) { return *((unsigned char*)&TPanel_Limits+Pos)                                                                    ;}
void           Reset_TPanel_Limits   ( void                                  ) {
//   Save_Eeprom_String((unsigned char*)&TPanel_Limits,"\xFE\x00\xFE\x00\xFE\xFE",sizeof(struct TPanel_Limits_Struct));
}
void           Save_TPanel_All_Limits(struct TPanel_Limits_Struct* Limits ) {
//   Save_Eeprom_String((unsigned char*)&TPanel_Limits,(unsigned char*) Limits,sizeof(struct TPanel_Limits_Struct));
}
void           Save_TPanel_Limits    ( unsigned char Pos,unsigned char Limit ) {
//   Save_Eeprom_Char ( (unsigned char* )&TPanel_Limits+Pos,Limit);
}
void  Save_TPanel_Default_Limits ( void ) {
//   Save_TPanel_All_Limits ( &Default_TPanel_Limits );
//   Overplay_Memo_Saved ( );
}
//---------------------------------------------------------------------------
unsigned char Touched      (void)
{
 return Tp.Touched=(Tp.Button ||(Tp.X<=TPanel_Limits.Max_X && Tp.X>=TPanel_Limits.Min_X && Tp.Y<=TPanel_Limits.Max_Y && Tp.Y>=TPanel_Limits.Min_Y))?1:0;
}
unsigned char TPanel_Scaled_X( void ) { return Tp.X_Scaled;}
unsigned char TPanel_Scaled_Y( void ) { return Tp.Y_Scaled;}
unsigned char TPanel_X       ( void ) { return Tp.X       ;}
unsigned char TPanel_Y       ( void ) { return Tp.Y       ;}
//--------------------------------------------------------------------
void Scale_X(void)
{ 
 Tp.X_Scaled=( Tp.X>TPanel_Limits.Min_X )?(Tp.X-TPanel_Limits.Min_X):0 ;//corre el offset teniendo en cuenta que no de negativo, por si eventualmente llega un valor menor que el minimo...
 Tp.X_Scaled=( (unsigned int                   )Tp.X_Scaled*63)/TPanel_Limits.Delta_X;//
}
void Scale_Y(void)
{
 Tp.Y_Scaled=( Tp.Y>TPanel_Limits.Min_Y )?(Tp.Y-TPanel_Limits.Min_Y):0  ;
 Tp.Y_Scaled=( (unsigned int                   )Tp.Y_Scaled*127)/TPanel_Limits.Delta_Y;
}
//---------------------------------------------------------
void Set_Y(void)
{
// Free_TPanel_Y( );//libera el eje y para que quede flotante y se pueda medir...
// Feed_TPanel_X( );//alimenta el eje X..
}
void Read_Y_And_Set_X      (void)
{
// Tp.Y=Read_Adc( TPanel_Y_Adc_Channel );//lee la respuesta del eje Y...
// Free_TPanel_X(                      );//libera el eje X para que quede flotante y se pueda medir...
// Feed_TPanel_Y(                      );//alimenta el eje Y...
}
void Read_X_And_Set_Y      (void)
{
// Send_VData2Serial(2,&Tp.X);        //debug
// Tp.X=Read_Adc( TPanel_X_Adc_Channel )                 ;//lee la respuesta del eje X...
// Free_TPanel_Y(                      )                 ;//libera el eje y para que quede flotante y se pueda medir...
// Feed_TPanel_X(                      )                 ;//alimenta el eje X...
// Index=Touched(                      )| (Tp.Touched<<1);//Ojo que Touched debe ejecutarse despues de preguntar por To.Touchesd, ya que lo modifica!! 0x00 RIen, 0x01 no estaba tocado pero ahora si->On_Click. 0x03 estaba tocado y ahora tambien->On_Drag, 0x02 estaba tocado y ahora se solto->on_release
}
//------------------------
void On_Click        (void)            //escala y busca algun handler
{
// Scale_X           (                                          );
// Scale_Y           (                                          );
// Tp.Last_Button=Tp.Button                                      ;
// Delay_Before_Drag=DELAY_BEFORE_DRAG_FILTER                    ;
//// Add_Select_Frame  (                                          );
// Find_Event_Handler( Tp.Last_Button,Tp.X_Scaled,Tp.Y_Scaled,0 );
}
void On_Drag         (void)            //escala y busca un handler
{
// if(Delay_Before_Drag) Delay_Before_Drag--;
//  else
//  {
//   Scale_X           (                                     );
//   Scale_Y           (                                     );
//   Tp.Last_Button=Tp.Button                                 ;
//   Find_Event_Handler( Tp.Button,Tp.X_Scaled,Tp.Y_Scaled,1 );
//  }
}
void On_Release         (void)
{
// Del_Select_Frame();
// if(Find_Event_Handler(Tp.Last_Button,Tp.X_Scaled,Tp.Y_Scaled,2)) Play_Beep();   //busca un handler con la ultima coordenada conocida, porque se acaba de soltar!!
}
void Read_Buttons    (void)   {}
//--------------------------------------------------------------------
void Init_Tpanel(void)
{
   TPanel_Sm=Initializing;
   Tp.Touched=0;
   CLOCK_EnableClock  ( kCLOCK_PortA                         );

   PORT_SetPinMux     ( PORTA ,0  ,kPORT_MuxAsGpio           ); // YU
   //PORT_Pullup_Enable ( PORTA ,0                             );
   GPIO_PinInit_As_In ( GPIOA ,0                             );

   PORT_SetPinMux     ( PORTA ,1  ,kPORT_MuxAsGpio           ); // XR
   GPIO_PinInit_As_Out ( GPIOA ,1,1                            );
//   PORT_Pullup_Enable ( PORTA ,1                             );
   //GPIO_PinInit_As_In ( GPIOA ,1                             );

   PORT_SetPinMux     ( PORTA ,15 ,kPORT_PinDisabledOrAnalog ); // YD
   GPIO_PinInit_As_In ( GPIOA ,15                            );

   PORT_SetPinMux     ( PORTA ,16  ,kPORT_MuxAsGpio           ); // XR
   //PORT_SetPinMux     ( PORTA ,16 ,kPORT_PinDisabledOrAnalog ); // XL
   //GPIO_PinInit_As_In ( GPIOA ,16                            );
   GPIO_PinInit_As_Out ( GPIOA ,16,0                            );
}
//--------------------------------------------------------------------
void Read_Y_And_Set_X_And_Read_Buttons (void)   {Read_Y_And_Set_X();Read_Buttons();}
//----------------------------------------------------
State Initializing   [ ]=
{
   { ANY_Event          ,Set_Y                             ,Reading_Y       },
};
State Reading_Y      [ ]=
{
   { ANY_Event          ,Read_Y_And_Set_X_And_Read_Buttons ,Reading_X       },
};
State Reading_X      [ ]=
{
   { ANY_Event          ,Read_X_And_Set_Y                  ,Testing_Touched },
};
State Testing_Touched[ ]=
{
//   { None_Touched_Event ,Read_Y_And_Set_X_And_Read_Buttons ,Reading_X       },
//   { Click_Event        ,On_Click                          ,Reading_Y       },
//   { Drag_Event         ,On_Drag                           ,Reading_Y       },
//   { Released_Event     ,On_Release                        ,Reading_Y       },
   { ANY_Event          ,Read_Y_And_Set_X_And_Read_Buttons ,Reading_X       },
};
//----------------------------------------------------
