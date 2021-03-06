#include "state_machine.h"
#include "events.h"
#include "display_phisical.h"
#include "display_layers.h"
#include "display_pics.h"
#include "schedule.h"
#include "str.h"
#include "type_conversion.h"
#include "everythings.h"
#include "welcome_pic.h"
#include "mask_pic.h"
#include "debug.h"
#include "tpanel.h"
#include "bkgd_pic.h"
#include "serial_tx.h"
//
static State
   Idle        [ ],
   Updating    [ ],
   Clearing_Lcd[ ],
   Aborting    [ ];
//------------------------------------------------------------------------------------
struct Struct_Pic_Layer Pic_Layers[MAX_PICS+1];
unsigned char           Pic_Layers_Used       ;
unsigned char           Layer_Modified        ;
unsigned char           Actual_Layer          ;
unsigned char           Watermark_Layer       ;
State*                  Display_Layers_Sm     ;
// -------------------------------------------------------------------------------------
void Set_Pic_In_Hole(unsigned char Hole,struct Struct_Pic* Pic)
{
 if(Pic_Layers_Used<MAX_PICS)
  {
   String_Copy_Backward((unsigned char*)&Pic_Layers[Hole],(unsigned char*)&Pic_Layers[Hole+1],((Pic_Layers_Used-Hole)*sizeof(struct Struct_Pic_Layer))); // mara-villa. copia usando string copy todo de una,, un casillero...
   Pic_Layers[Hole].Pic          = Pic;
   Pic_Layers[Hole].Blink_State  = 1;
   Pic_Layers[Hole].Blink_Period = Pic_Layers[Hole].Actual_Blink = Pic->Blink_Period;
   Pic_Layers_Used++;
   Layer_Structure_Modified();
   Pic_Layers[Hole].Pic->Events->Handler[0]();
  }
}
void Cap_Hole(unsigned char Hole)
{
 if(Hole<Pic_Layers_Used)
 {
  void (*F)(void)= Pic_Layers[Hole].Pic->Events->Handler[2];
  Pic_Layers_Used--;
  String_Copy_Forward((unsigned char*)&Pic_Layers[Hole+1],
                      (unsigned char*)&Pic_Layers[Hole],
                      ((Pic_Layers_Used-Hole)*sizeof(struct Struct_Pic_Layer))); // mara-villa. copia usando string copy todo de una,, un casillero...
  Layer_Structure_Modified();
  F();                                                                           // se ejecuta la funcion destructor al fina porque sino corrompe todo
 }
}
void Move_Layer2Down(unsigned char Layer)     // mueve el layer una posicino hacia abajo...
{
 if(Layer && Pic_Layers_Used>Layer)           // debera estar al menos en la posicion 1 y haber mas de 1 pic.. sino no tiene sentido la operacion...
  {
   struct Struct_Pic_Layer Aux1_Pic,Aux2_Pic; // auxiliares para simplificar al compilador...
   Aux1_Pic            = Pic_Layers[Layer-1];
   Aux2_Pic            = Pic_Layers[Layer];
   Pic_Layers[Layer-1] = Aux2_Pic;            // el de abajo con lo de arriba
   Pic_Layers[Layer]   = Aux1_Pic;            // el de arriba con lo de abajo
   Layer_Structure_Modified();
  }
}
void Move_Layer2Up(unsigned char Layer)       // muevo el layer una posicion hacia arriba
{
 Layer++;                                     // es un truco para que no salte C_reg en la comparacion siguiente
 if(Pic_Layers_Used>Layer)                    // debera haber al menos un pic extra que el solicitado...
  {
   struct Struct_Pic_Layer Aux1_Pic,Aux2_Pic; // auxiliares para simplificar al compilador...
   Aux1_Pic            = Pic_Layers[Layer];
   Aux2_Pic            = Pic_Layers[Layer-1];
   Pic_Layers[Layer]   = Aux2_Pic;            // arriba lo de abajo...
   Pic_Layers[Layer-1] = Aux1_Pic;            // abajo lo de arriba
   Layer_Structure_Modified();
  }
}
                                                                                                                                                         // -------------------------------------------------------------------------------------
bool Find_Event_Handler ( uint8_t Button,uint16_t X,uint16_t Y,uint8_t Handler ) // se ejecuta cuando se presiona el touch panel, se lo draga o se lo suelta...compara todos los layers en busca de insteersecciones...
{
   unsigned char Layer=Pic_Layers_Used,Count;                    // auxiliar para contar el numero de layers ocupados y la cantidad de zonas activas que tiene cada pic......
   struct Struct_Pic_Events *Events;                             // auxiiar que apunta a la lista de handlres para no tener que desreferenciar cada vez que se preguna por un campo....se puede pero salta C_reg...
   while(Layer--) {                                              // para todos los layers (pics) empezando por el de mas alto nivel....
      Events=Pic_Layers[Layer].Pic->Events+1;
      for(Count=Pic_Layers[Layer].Pic->ECount-1;Count;Count--) { // desreferencia la lista de eventos, y compara mientras count indique que hay mas eventos OJO SE SALTEA el primer evento, porque esta reservado para "On_Create", arranca desde el 2do. Todas las pics deberan tener al menos 1 evento que se ejecuta al crearse, pero aca no se lo considera... que procesar, pasado eso termina y pasa al layer inferior...
         if( Button==Events->Button ||
            (X>=Events->Pos.XL &&
             X<=Events->Pos.XR &&
             Y>=Events->Pos.YU &&
             Y<=Events->Pos.YL)
            || Events->Button==Any_Button) {                     // si apreto el boton esperado OOOO si apunto dentro de la zona indicada por el evento OOOOO se esperaba que aprete cualquier boton...,
             if(Events->Draw_Box==true)
                Set_Mask_Pic(&Events->Pos);                      // solo se dibuja en recuadro si asi lo pidiera el evento...
             Events->Handler[Handler]();
             return Events->Handler[Handler]!=Rien;
         }                                                       // ejecuta el handler en funcion si es on,drag o release y sale inmediatamente. Solo se ejecuta una funcion por cada apuntada.maximo..minimo ninguna...
         Events++;
      }
   }
return 0;
}

void Blink(void)
{
 unsigned char Layer=Pic_Layers_Used;
 while(Layer--)
   if(Pic_Layers[Layer].Blink_Period)
      if(!--Pic_Layers[Layer].Actual_Blink)
     {
      Pic_Layers[Layer].Actual_Blink=Pic_Layers[Layer].Blink_Period;
      if(Pic_Layers[Layer].Pic->Events->Handler[1]==Rien)
         Pic_Layers[Layer].Blink_State^=0x01;
      else 
         Pic_Layers[Layer].Pic->Events->Handler[1]();
//      Layer_Structure_Modified();
     }
}
// -------------------------------------------------------------------------------------
void Block_Pic_Events   (struct Struct_Pic* Pic)         {Pic->ECount=1;}
void Unblock_Pic_Events (struct Struct_Pic* Pic,unsigned char Ecount)   {Pic->ECount=Ecount;}
// -------------------------------------------------------------------------------------
unsigned char Search_Pic_Pos  (struct Struct_Pic* Pic)
{
 unsigned char i;
 for(i=0;i<Pic_Layers_Used && Pic_Layers[i].Pic!=Pic;i++);
 return i;
}
// -------------------------------------------------------------------------------------
void Update_Blink_Pic(struct Struct_Pic* Pic)
{
 unsigned char Layer=Search_Pic_Pos(Pic);
 Pic_Layers[Layer].Actual_Blink=Pic_Layers[Layer].Blink_Period;
}
void Do_Blink_Pic(struct Struct_Pic* Pic,unsigned int Period)
{
 unsigned char Layer=Search_Pic_Pos(Pic);
 Pic_Layers[Layer].Blink_Period=Pic_Layers[Layer].Actual_Blink=Period;
 Pic_Layers[Layer].Blink_State=0;                                                                                                                        // para que salte inmediatamente..
 Layer_Structure_Modified();
}
void Dont_Blink_Pic(struct Struct_Pic* Pic)
{
 unsigned char Layer=Search_Pic_Pos(Pic);
 Pic_Layers[Layer].Blink_Period=0;
 Pic_Layers[Layer].Blink_State=1;                                                                                                                        // para que salte inmediatamente..
 Layer_Structure_Modified();
}

void Dont_Blink_Any_Pic(void)
{
 unsigned char i;
 for(i=0;i<Pic_Layers_Used;i++) {Pic_Layers[i].Blink_State=1;Pic_Layers[i].Blink_Period=0;}
 Layer_Structure_Modified();
}
void Do_Blink_Only_Pic(struct Struct_Pic* Pic,unsigned int Period)   {Dont_Blink_Any_Pic();Do_Blink_Pic(Pic,Period);}

                                                                                                                                                         // -------------------------------------------------------------------------------------
void Move_Pic2Higth ( unsigned char Bits, struct Struct_Pic* Pic ) { }                                                                                   // Pic_Layers[Search_Pic_Pos(Pic)].Pic->Upper_Y+=Bits;Layer_Structure_Modified();}
void Move_Pic2Low   ( unsigned char Bits, struct Struct_Pic* Pic ) { }                                                                                   // Pic_Layers[Search_Pic_Pos(Pic)].Pic->Upper_Y-=Bits;Layer_Structure_Modified();}
void Move_Pic2Rigth ( unsigned char Bits, struct Struct_Pic* Pic ) { }                                                                                   // Pic_Layers[Search_Pic_Pos(Pic)].Pic->Left_X+=Bits;Layer_Structure_Modified();}
void Move_Pic2Left  ( unsigned char Bits, struct Struct_Pic* Pic ) { }                                                                                   // Pic_Layers[Search_Pic_Pos(Pic)].Pic->Left_X-=Bits;Layer_Structure_Modified();}
                                                                                                                                                         // -------------------------------------------------------------------------------------
void Move_Pic2Up              ( struct Struct_Pic* Pic                     ) { Move_Layer2Up  (Search_Pic_Pos(Pic))                                        ;}
void Move_Pic2Down            ( struct Struct_Pic* Pic                     ) { Move_Layer2Down(Search_Pic_Pos(Pic))                                        ;}
void Move_Pic2Top             ( struct Struct_Pic* Pic                     ) { Del_Pic(Pic)                                                                ;Add_Pic_On_Top(Pic)                                         ;}
void Move_Pic2Bottom          ( struct Struct_Pic* Pic                     ) { Del_Pic(Pic)                                                                ;Add_Pic_On_Bottom(Pic)                                      ;}
void Add_Pic_On_Top           ( struct Struct_Pic* Pic                     ) { Set_Pic_In_Hole(Pic_Layers_Used,Pic)                                        ;}
void Add_Pic_On_Bottom        ( struct Struct_Pic* Pic                     ) { Set_Pic_In_Hole(0,Pic)                                                      ;}
void Add_Pic_On_Layer         ( struct Struct_Pic* Pic,unsigned char Layer ) { Set_Pic_In_Hole(Layer,Pic)                                                  ;}
void Del_Pic                  ( struct Struct_Pic* Pic                     ) { Cap_Hole(Search_Pic_Pos(Pic))                                               ;}
void Del_All_Layers           ( void                                       ) {
   Pic_Layers_Used=0           ;
   Layer_Structure_Modified ( );
}
void Layer_Clr_Lcd ( void ) {
   if(!(Layer_Modified&0x04)) {
      Atomic_Insert_Event ( Clear_Lcd_Event,Display_Layers( )); //quiero que entre primero el clr, por eso el insert
      Layer_Modified|=0x04;
   }
};
void Layer_Structure_Modified ( void ) {
   if(!(Layer_Modified&0x02)) {
      Atomic_Send_Event ( Structure_Modified_Event,Display_Layers( ));
      Layer_Modified|=0x02;
   }
};
void Does_Layer_Modified ( void )
{
        if(Layer_Modified&0x04)
           Atomic_Send_Event(Clear_Lcd_Event,Display_Layers());
   else if(Layer_Modified&0x02)
           Atomic_Send_Event(Structure_Modified_Event,Display_Layers());
}
unsigned char  Layer_Used       ( void ) { return Pic_Layers_Used            ;}
void           Print_Layer_Used ( void ) { Send_Int_NLine2Serial(Layer_Used());}
// -------------------------------------------------------------------------------------
State**  Display_Layers      ( void ) { return &Display_Layers_Sm                    ;} // devuelve la direccion de la maquina de estados Everythings para poder mandarle mensajes.
void     Display_Layers_Rti  ( void ) { Atomic_Send_Event(ANY_Event,Display_Layers());}
void     Init_Display_Layers ( void )
{
 Pic_Layers_Used      = 0;
 Layer_Modified       = 0;
 Display_Layers_Sm    = Idle;
 Watermark_Layer      = 0;
 New_Periodic_Func_Schedule(10,Blink);
 Add_Welcome();
}
                                                                                                                                                         // -------------------------------------------------------------------------------------
void Next_Layer      (void)
{
   for(;Actual_Layer<Pic_Layers_Used && !Pic_Layers[Actual_Layer].Blink_State;Actual_Layer++)                                                            // salteo los que estan deshabilitados.
      ;
   if(Actual_Layer<Pic_Layers_Used) {
          Pic2Lcd(Pic_Layers[Actual_Layer].Pic);
          Actual_Layer++;
   }
   else
      Atomic_Send_Event(All_Updated_Event,Display_Layers());
}
// -------------------------------------------------------------------------------------
void Send_Next_Layer_Event ( void ) { Atomic_Send_Event(Next_Layer_Event,Display_Layers());}
void Set_Watermark_Layer   ( void ) { Watermark_Layer = Pic_Layers_Used                   ;}
void Reset_Watermark_Layer ( void ) { Watermark_Layer = 0                                 ;}
void Idle_Structure_Modified    (void)
{
   Actual_Layer=Watermark_Layer;
   Layer_Modified&=~0x02;
   Send_Next_Layer_Event();
}
void Idle_Clear_Lcd    (void)
{
   Layer_Modified&=~0x04;
   Pic2Lcd(Read_Bkgd_Black_Pic());
}
void All_Displayed      (void)   {Atomic_Send_Event(All_Displayed_Event,Display_Layers());}
// -------------------------------------------------------------------------------------
void Update_Mask_Pic_And_Does_Layer_Modified ( void ) { Update_Mask_Pic();Does_Layer_Modified();}
// -------------------------------------------------------------------------------------
static State Aborting    [ ] RODATA =
{
{ Next_Sub_Pic_Event       ,Does_Layer_Modified          ,Idle         },
{ Next_Layer_Event         ,Does_Layer_Modified          ,Idle         },
{ ANY_Event                ,Rien                         ,Aborting     },
                                                                       };
static State Idle        [ ] RODATA =
{
{ Structure_Modified_Event ,Idle_Structure_Modified      ,Updating     },
{ Clear_Lcd_Event          ,Idle_Clear_Lcd               ,Clearing_Lcd },
{ ANY_Event                ,Rien                         ,Idle         },
                                                                       };
static State Updating    [ ] RODATA =
{
{ Next_Sub_Pic_Event       ,Sub_Pic2Lcd                             ,Updating } ,
{ Next_Layer_Event         ,Next_Layer                              ,Updating } ,
{ All_Updated_Event        ,Update_Mask_Pic_And_Does_Layer_Modified ,Idle     } ,
{ Structure_Modified_Event ,Rien                                    ,Aborting } ,
{ Clear_Lcd_Event          ,Rien                                    ,Aborting } ,
{ ANY_Event                ,Rien                                    ,Updating } ,
};
static State Clearing_Lcd[ ] RODATA =
{
{ Next_Sub_Pic_Event       ,Sub_Pic2Lcd                  ,Clearing_Lcd },
{ Next_Layer_Event         ,Idle_Structure_Modified      ,Updating     },
{ Structure_Modified_Event ,Rien                         ,Aborting     },
{ Clear_Lcd_Event          ,Rien                         ,Aborting     },
{ ANY_Event                ,Rien                         ,Clearing_Lcd },
                                                                       };
