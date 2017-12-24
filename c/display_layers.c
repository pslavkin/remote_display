#include "state_machine.h"
#include "events.h"
#include "display_phisical.h"
#include "serial_tx.h"
#include "display_layers.h"
#include "display_pics.h"
#include "schedule.h"
#include "str.h"
#include "buttons.h"
#include "type_conversion.h"
#include "debug.h"

static State 
	Off	[],
	Idle	[],
	Updating[];

//------------------------------------------------------------------------------------
struct Struct_Pic_Layer	Pic_Layers[MAX_PICS+1];
unsigned char 		Pic_Layers_Used;
unsigned char 		Layer_Modified;
unsigned char 		Actual_Layer;
unsigned char 		Handler_Layer;			//auxiliar para contar el numero de layers ocupados que tiene cada pic en la funcino Find_Event_Handler, es global porque la usan las funcinoes String_List_HHandlers......
State*	 	 	Display_Layers_Sm;
//-------------------------------------------------------------------------------------
void Layer2Bank		(struct Struct_Pic_Layer* Pic_Layer)
{
 struct Struct_Pic_Layer Layer=*Pic_Layer;
 struct Struct_Pic* Pic=Layer.Pic;
 unsigned char* String=Pic->List.String[Layer.Item_Index].String;
 unsigned char CR,i,Line,Item_Index=Layer.Item_Index;
// unsigned char ** Disp_Bank=Read_Disp_Bank(); 

 for(Line=Pic->StartY;Line<Pic->EndY;Line++) {
	for(CR=Pic_Layer->HScroll[Line],i=Pic->StartX;i<Pic->EndX;i++,CR++) {
    		if(String[CR]==0x00) CR=0;				//si llego el NULL, vuelve desde el inicio del renglonn
    		Disp_Bank[Line][i]=String[CR];
    	}
   	String=Pic->List.String[++Item_Index].String;
  }
}
//-------------------------------------------------------------------------------------
void Set_Pic_In_Hole_Index(unsigned char Hole,struct Struct_Pic* Pic,unsigned char Index)
{
 if(Pic_Layers_Used<MAX_PICS) 
 {
  unsigned char i;
  String_Copy_Backward((unsigned char*)&Pic_Layers[Hole],(unsigned char*)&Pic_Layers[Hole+1],((Pic_Layers_Used-Hole)*sizeof(struct Struct_Pic_Layer)));	//mara-villa. copia usando string copy todo de una,, un casillero...
  Pic_Layers[Hole].Pic=Pic;
  Pic_Layers[Hole].Blink_State=0x07;   		//por defecto habilitada y encendida y activa y con eventos periodicos
  Pic_Layers[Hole].Actual_Blink=Pic->Blink_Period;
  Pic_Layers[Hole].Actual_HScroll=Pic->HScroll_Period*4;
  for(i=0;i<DISP_LINES;i++) Pic_Layers[Hole].HScroll[i]=0;
  Pic_Layers[Hole].Item_Index=Index;
  Pic_Layers[Hole].Handler_Index=Index;
  Pic_Layers_Used++;
  Layer_Structure_Modified();
  Pic_Layers[Hole].Pic->Events->Handler[0]();   
 }
}
void Set_Pic_In_Hole(unsigned char Hole,struct Struct_Pic* Pic)	{Set_Pic_In_Hole_Index(Hole,Pic,0);}
//---------------
void Cap_Hole(signed char Hole)
{
if(Hole>=0) 
 {
  void (*Func)(void)=Pic_Layers[Hole].Pic->Events->Handler[2];   												//hay que ejecutar el destructor DESPUES de elminiar el pic, porque si el destructor TAMBNIEN elimina un pic de mas abajo, nos desarma todo el esquema...
  Pic_Layers_Used--;
  String_Copy_Forward((unsigned char*)&Pic_Layers[Hole+1],(unsigned char*)&Pic_Layers[Hole],((Pic_Layers_Used-Hole)*sizeof(struct Struct_Pic_Layer)));		//mara-villa. copia usando string copy todo de una,, un casillero...
  Func();																			//Se puede dar recursividad si la funcion de "destroy" es tambien un Cap_Hole!! pero funca.. mientras no colapse el stack...	
  Layer_Structure_Modified();
 }
}
void Move_Layer2Down(unsigned char Layer)	//mueve el layer una posicino hacia abajo...
{
 if(Layer && Pic_Layers_Used>Layer)  		//debera estar al menos en la posicion 1 y haber mas de 1 pic.. sino no tiene sentido la operacion...
 {
  struct Struct_Pic_Layer Aux1_Pic,Aux2_Pic;	//auxiliares para simplificar al compilador...
  Aux1_Pic=Pic_Layers[Layer-1];		
  Aux2_Pic=Pic_Layers[Layer];
  Pic_Layers[Layer-1]=Aux2_Pic;			//el de abajo con lo de arriba
  Pic_Layers[Layer]=Aux1_Pic;			//el de arriba con lo de abajo
  Layer_Structure_Modified();
 }
}
void Move_Layer2Up(unsigned char Layer)		//muevo el layer una posicion hacia arriba
{
 Layer++;					//es un truco para que no salte C_reg en la comparacion siguiente
 if(Pic_Layers_Used>Layer)  			//debera haber al menos un pic extra que el solicitado...
 {
  struct Struct_Pic_Layer Aux1_Pic,Aux2_Pic;	//auxiliares de compilador...
  Aux1_Pic=Pic_Layers[Layer];
  Aux2_Pic=Pic_Layers[Layer-1];
  Pic_Layers[Layer]=Aux2_Pic;			//arriba lo de abajo... 
  Pic_Layers[Layer-1]=Aux1_Pic;			//abajo lo de arriba
  Layer_Structure_Modified();
 }
}
void Move_Layer2Top		(unsigned char Layer)
{
 Layer++;					//es un truco para que no salte C_reg en la comparacion siguiente
 if(Pic_Layers_Used>Layer)  			//debera haber al menos un pic extra que el solicitado...
 {
  struct Struct_Pic_Layer Aux_Pic;		//auxiliares de compilador...
  Aux_Pic=Pic_Layers[Layer-1];
  String_Copy_Forward((unsigned char*)&Pic_Layers[Layer],(unsigned char*)&Pic_Layers[Layer-1],((Pic_Layers_Used-Layer)*sizeof(struct Struct_Pic_Layer)));		//mara-villa. copia usando string copy todo de una,, un casillero...
  Pic_Layers[Pic_Layers_Used-1]=Aux_Pic;	//arriba lo del agujero
  Layer_Structure_Modified();
 }
} 
void Move_Layer2Bottom		(unsigned char Layer)	
{
 if(Layer && Pic_Layers_Used>Layer)  		//debera estar al menos en la posicion 1 y haber mas de 1 pic.. sino no tiene sentido la operacion...
 {
  struct Struct_Pic_Layer Aux_Pic;		//auxiliares para simplificar al compilador...
  Aux_Pic=Pic_Layers[Layer];		
  String_Copy_Backward((unsigned char*)&Pic_Layers[0],(unsigned char*)&Pic_Layers[1],(Layer*sizeof(struct Struct_Pic_Layer)));		//mara-villa. copia usando string copy todo de una,, un casillero...
  Pic_Layers[0]=Aux_Pic;			//el de arriba con lo de abajo
  Layer_Structure_Modified();
 }
} 
//-------------------------------------------------------------------------------------
void Event_Handler_Actual_Event(void)
{	
	unsigned int Event=Actual_Event();
	Find_Event_Handler(*(unsigned char*)&Event+0,*(unsigned char*)&Event+1);						//uso la parte alta como comando y la baja para la posicion del hanlder ... como para no mandar otro byte mas y mantener la comunicacion cortita entre los host... 
}
void Event_Handler0_Actual_Event(void)	{   Find_Event_Handler(Actual_Event()-0,0);}
void Event_Handler1_Actual_Event(void)	{if(Find_Event_Handler(Actual_Event()-1,1)) {};}//Beep();}
void Event_Handler2_Actual_Event(void)	{if(Find_Event_Handler(Actual_Event()-2,2)) {};}//Beep();}

unsigned char Find_Event_Handler(unsigned char Button,unsigned char Handler)						//se ejecuta cuando se presiona el touch panel, se lo draga o se lo suelta...compara todos los layers en busca de insteersecciones...
{
	if(Read_Tandem()&0x01) {
		struct Struct_Pic_Events *Events;											//auxiiar que apunta a la lista de handlres para no tener que desreferenciar cada vez que se preguna por un campo....se puede pero salta C_reg...
		unsigned char Count;													//auxiliar para contar el numero de zonas activas que tiene cada pic......
		Handler_Layer=Pic_Layers_Used;												//se definio lobal para que la usen los string_list_handlers

		while(Handler_Layer--)													//para todos los layers (pics) empezando por el de mas alto nivel....
			if(Pic_Layers[Handler_Layer].Blink_State&0x04)
				for(Count=Pic_Layers[Handler_Layer].Pic->ECount,Events=Pic_Layers[Handler_Layer].Pic->Events+1;--Count;Events++)  	//desreferencia la lista de eventos, y compara mientras count indique que hay mas eventos OJO SE SALTEA el primer evento, porque esta reservado para "On_Create", arranca desde el 2do. Todas las pics deberan tener al menos 1 evento que se ejecuta al crearse, pero aca no se lo considera... que procesar, pasado eso termina y pasa al layer inferior...
					if(Events->Button==Button || Events->Button==Any_Button_Code) {							//si apreto el boton esperado OOOO si apunto dentro de la zona indicada por el evento OOOOO se esperaba que aprete cualquier boton..., 
						Events->Handler[Handler]();
						return Events->Handler[Handler]!=Rien;														//se usa principalmente para los beeps..
						}
		return 0;
	}
	return 0;
}
void String_List_Handler	(unsigned char Pos)				{Pic_Layers[Handler_Layer].	Pic->List.String[Pic_Layers[Handler_Layer].		Handler_Index].Handler[Pos]();}
void Pic_String_List_Handler	(struct Struct_Pic* Pic,unsigned char Pos) 	{				Pic->List.String[Pic_Layers[Search_Pic_Pos(Pic)].	Handler_Index].Handler[Pos]();}

void String_List_Handler0	(void)			{String_List_Handler(0);}
void String_List_Handler1	(void)			{String_List_Handler(1);}
void String_List_Handler2	(void)			{String_List_Handler(2);}

void Pic_String_List_Handler0	(struct Struct_Pic* Pic) 			{Pic_String_List_Handler(Pic,0);}
void Pic_String_List_Handler1	(struct Struct_Pic* Pic) 			{Pic_String_List_Handler(Pic,1);}
void Pic_String_List_Handler2	(struct Struct_Pic* Pic) 			{Pic_String_List_Handler(Pic,2);}
//----------------------------------------------
void Blink(void)
{
	Handler_Layer=Pic_Layers_Used;
	while(Handler_Layer--)		
		if(Pic_Layers[Handler_Layer].Pic->Blink_Period)
			if(!--Pic_Layers[Handler_Layer].Actual_Blink) {
				Pic_Layers[Handler_Layer].Actual_Blink=Pic_Layers[Handler_Layer].Pic->Blink_Period;
				if(Pic_Layers[Handler_Layer].Blink_State&0x08) Pic_Layers[Handler_Layer].Blink_State^=0x01;
				Pic_Layers[Handler_Layer].Pic->Events->Handler[1]();
				Layer_Info_Modified();
			}
}
void Refresh_Pic_Blink(struct Struct_Pic *Pic) 	
{
	unsigned char Layer=Search_Pic_Pos(Pic);
	Pic_Layers[Layer].Actual_Blink=Pic->Blink_Period;
}
void Refresh_Blink(void) 	{Pic_Layers[Handler_Layer].Actual_Blink=Pic_Layers[Handler_Layer].Pic->Blink_Period;}
void HScroll(void)
{
 unsigned char Layer=Pic_Layers_Used;
 while(Layer--)		
  if(Pic_Layers[Layer].Pic->HScroll_Period && Pic_Layers[Layer].Blink_State&0x02)	//solo si esta habilitada
   if(!--Pic_Layers[Layer].Actual_HScroll)
   {
    unsigned char i;
    struct Struct_String_List* String_List=&Pic_Layers[Layer].Pic->List.String[Pic_Layers[Layer].Item_Index];
    Pic_Layers[Layer].Actual_HScroll=Pic_Layers[Layer].Pic->HScroll_Period;
    for(i=Pic_Layers[Layer].Pic->StartY;i<Pic_Layers[Layer].Pic->EndY;i++,String_List++)
    	if(String_List->Scroll)
    		if(String_List->String[++Pic_Layers[Layer].HScroll[i]]==0x00) Pic_Layers[Layer].HScroll[i]=0;
    Layer_Info_Modified();
   }
}
//-------------------------------------------------------------------------------------
signed char Search_Pic_Pos	(struct Struct_Pic* Pic)
{
	signed char i;
	for(i=(Pic_Layers_Used-1);i>=0 && Pic_Layers[i].Pic!=Pic;i--)
		;
	return i;
}
unsigned char Does_Pic_Exist	(struct Struct_Pic* Pic)
{
 return Search_Pic_Pos(Pic)>=0;
}
unsigned char Is_Pic_On_Top	(struct Struct_Pic* Pic)	
{
 return Search_Pic_Pos(Pic)==(Pic_Layers_Used-1);
} 
//-------------------------------------------------------------------------------------
void Do_Blink_Pic(struct Struct_Pic* Pic)
{
 unsigned char Layer=Search_Pic_Pos(Pic);
 Pic_Layers[Layer].Actual_Blink=Pic_Layers[Layer].Pic->Blink_Period;
 Pic_Layers[Layer].Blink_State|= 0x08;						//con esto habilito para que blinkee
 Pic_Layers[Layer].Blink_State&=~0x01;						//para que salte inmediatamente..
 Layer_Info_Modified();
}
void Dont_Blink_Pic(struct Struct_Pic* Pic)
{
 unsigned char Layer=Search_Pic_Pos(Pic);
 Pic_Layers[Layer].Blink_State&=~0x08;						//con esto habilito para que blinkee
 Pic_Layers[Layer].Blink_State|= 0x01;						//para que salte inmediatamente..
 Layer_Info_Modified();
}
void Dont_Blink_Any_Pic(void)
{
 unsigned char i;
 for(i=0;i<Pic_Layers_Used;i++) {Pic_Layers[i].Blink_State|=0x01;Pic_Layers[i].Pic->Blink_Period=0;}
 Layer_Info_Modified();
}
void Do_Blink_Only_Pic(struct Struct_Pic* Pic)		{Dont_Blink_Any_Pic();Do_Blink_Pic(Pic);}
//-------------------------------------------------------------------------------------
void VnScroll2Down_Layer(unsigned char Layer,unsigned char Times)
{
	if(Pic_Layers[Layer].Pic->List.String[Pic_Layers[Layer].Item_Index+Times].String[0]!=0x00) {
		unsigned char i;
		Pic_Layers[Layer].Item_Index 	+=Times;
		Pic_Layers[Layer].Handler_Index	+=Times;
		for(i=0;i<DISP_LINES;i++) Pic_Layers[Layer].HScroll[i]=0;
		Pic_Layers[Layer].Actual_HScroll=Pic_Layers[Layer].Pic->HScroll_Period*4;
		Layer_Info_Modified();
		}
}
void VnScroll2Down_Pic(struct Struct_Pic* Pic,unsigned char Times)
{
	char Layer=Search_Pic_Pos(Pic);
	if(Layer>=0) VnScroll2Down_Layer(Layer,Times);
}
void V1Scroll2Down 	(void) 				{VnScroll2Down_Layer(Handler_Layer,1);}
void V1Scroll2Down_Pic 	(struct Struct_Pic *Pic) 	{VnScroll2Down_Pic(Pic,1);}
void V2Scroll2Down 	(void) 				{VnScroll2Down_Layer(Handler_Layer,2);}
void V3Scroll2Down_Pic 	(struct Struct_Pic *Pic) 	{VnScroll2Down_Pic(Pic,3);}
void V3Scroll2Down 	(void) 				{VnScroll2Down_Layer(Handler_Layer,3);}
void V4Scroll2Down 	(void) 				{VnScroll2Down_Layer(Handler_Layer,4);}
void V5Scroll2Down 	(void) 				{VnScroll2Down_Layer(Handler_Layer,5);}
void V6Scroll2Down 	(void) 				{VnScroll2Down_Layer(Handler_Layer,6);}
void V7Scroll2Down 	(void) 				{VnScroll2Down_Layer(Handler_Layer,7);}
//----------------------------------------------------------------------------------------------------
void VnScroll2Up_Layer(unsigned char Layer,unsigned char Times)
{
	if(Pic_Layers[Layer].Item_Index>=Times) {
		 unsigned char i;
		 Pic_Layers[Layer].Item_Index		-=Times;
		 Pic_Layers[Layer].Handler_Index	-=Times;
		 for(i=0;i<DISP_LINES;i++) Pic_Layers[Layer].HScroll[i]=0;
		 Pic_Layers[Layer].Actual_HScroll=Pic_Layers[Layer].Pic->HScroll_Period*4;
		 Layer_Info_Modified();
		}
}
void VnScroll2Up_Pic(struct Struct_Pic* Pic,unsigned char Times)
{
	char Layer=Search_Pic_Pos(Pic);
	if(Layer>=0) VnScroll2Up_Layer(Layer,Times);
}
void V1Scroll2Up 	(void)				{VnScroll2Up_Layer(Handler_Layer,1);}
void V1Scroll2Up_Pic 	(struct Struct_Pic* Pic)	{VnScroll2Up_Pic(Pic,1);}
void V2Scroll2Up 	(void)				{VnScroll2Up_Layer(Handler_Layer,2);}
void V3Scroll2Up_Pic 	(struct Struct_Pic* Pic)	{VnScroll2Up_Pic(Pic,3);}
void V4Scroll2Up 	(void)				{VnScroll2Up_Layer(Handler_Layer,4);}
void V5Scroll2Up 	(void)				{VnScroll2Up_Layer(Handler_Layer,5);}
void V6Scroll2Up 	(void)				{VnScroll2Up_Layer(Handler_Layer,6);}
void V7Scroll2Up 	(void)				{VnScroll2Up_Layer(Handler_Layer,7);}
//-------------------------------------------------------------------------------------
void Set_Pic_Item_Index		(struct Struct_Pic* Pic,unsigned char Index)	
{
 unsigned char i;
 unsigned char Layer=Search_Pic_Pos(Pic);
 Pic_Layers[Layer].Item_Index=Index;
 for(i=0;i<DISP_LINES;i++) Pic_Layers[Layer].HScroll[i]=0;
 Pic_Layers[Layer].Actual_HScroll=Pic_Layers[Layer].Pic->HScroll_Period*4;
 Layer_Info_Modified();
} 
void Set_Actual_Pic_Item_Index	(unsigned char Index)	
{
 unsigned char i;
 Pic_Layers[Handler_Layer].Item_Index=Index;
 for(i=0;i<DISP_LINES;i++) Pic_Layers[Handler_Layer].HScroll[i]=0;
 Pic_Layers[Handler_Layer].Actual_HScroll=Pic_Layers[Handler_Layer].Pic->HScroll_Period*4;
 Layer_Info_Modified();
} 
unsigned char Read_Pic_Item_Index	(struct Struct_Pic* Pic)	
{
 unsigned char Layer=Search_Pic_Pos(Pic);
 return Pic_Layers[Layer].Item_Index;
}
unsigned char Read_Actual_Pic_Item_Index	(void)	
{
 return Pic_Layers[Handler_Layer].Item_Index;
}
unsigned char* Read_Pic_Item_String_Index 	(struct Struct_Pic* Pic,unsigned char Index)	
{
 unsigned char Layer=Search_Pic_Pos(Pic);
 return Pic_Layers[Layer].Pic->List.String[Index].String;
}
//-------------------
void Set_Pic_Handler_Index		(struct Struct_Pic* Pic,unsigned char Index)	
{
 unsigned char Layer=Search_Pic_Pos(Pic);
 Pic_Layers[Layer].Handler_Index=Index;
} 
void Set_Actual_Pic_Handler_Index	(unsigned char Index)	
{
 Pic_Layers[Handler_Layer].Handler_Index=Index;
} 
void Actual_Pic_Handler_Index2Down		(void)	
{
 Pic_Layers[Handler_Layer].Handler_Index++;
} 
void Actual_Pic_Handler_Index2Up		(void)	
{
 Pic_Layers[Handler_Layer].Handler_Index--;
} 
//-------------------------------------------------------------------------------------
void Set_Pic_Item_And_Handler_Index		(struct Struct_Pic* Pic,unsigned char Index)
{
	Set_Pic_Item_Index(Pic,Index);
	Set_Pic_Handler_Index(Pic,Index);
}
void Set_Actual_Pic_Item_And_Handler_Index	(struct Struct_Pic* Pic,unsigned char Index)
{
	Set_Actual_Pic_Item_Index(Index);
	Set_Actual_Pic_Handler_Index(Index);
}
//-------------------------------------------------------------------------------------
void Move_Pic2X_Y		(struct Struct_Pic* Pic,unsigned char X,unsigned char Y)
{
 Pic->StartX=X;
 Pic->EndX=X+(Pic->EndX-Pic->StartX);
 Pic->StartY=Y;
 Pic->EndY=Y+(Pic->EndY-Pic->StartY);
 Layer_Info_Modified();
} 
void Move_Pic2Up		(struct Struct_Pic* Pic)			{Move_Layer2Up  	(Search_Pic_Pos(Pic));} 
void Move_Pic2Down		(struct Struct_Pic* Pic)			{Move_Layer2Down	(Search_Pic_Pos(Pic));} 
void Move_Pic2Top		(struct Struct_Pic* Pic)			{Move_Layer2Top 	(Search_Pic_Pos(Pic));}
void Move_Pic2Bottom		(struct Struct_Pic* Pic)			{Move_Layer2Bottom	(Search_Pic_Pos(Pic));}
void Active_Pic			(struct Struct_Pic* Pic)			
{
 unsigned char Layer=Search_Pic_Pos(Pic);
 Pic_Layers[Layer].Blink_State|=0x04;	//la habilita para que se procesen los event handlers en esta pic
 Layer_Info_Modified();
}
void Pasive_Pic			(struct Struct_Pic* Pic)			
{
 unsigned char Layer=Search_Pic_Pos(Pic);
 Pic_Layers[Layer].Blink_State&=~0x04;  //no se le procesasn os event handlers en esta pic
 Layer_Info_Modified();
}
void Enable_Pic			(struct Struct_Pic* Pic)			
{
 unsigned char Layer=Search_Pic_Pos(Pic);
 Pic_Layers[Layer].Blink_State|=0x02;	//la habilita, con lo cual se procesan los scrolls horizontales, etc.
 Layer_Info_Modified();
}
void Disable_Pic		(struct Struct_Pic* Pic)			
{
 unsigned char Layer=Search_Pic_Pos(Pic);
 Pic_Layers[Layer].Blink_State&=~0x02;
 Layer_Info_Modified();
}
void Unhide_Pic			(struct Struct_Pic* Pic)			
{
 unsigned char Layer=Search_Pic_Pos(Pic);
 Pic_Layers[Layer].Blink_State|=0x01;			// si esto vale uno y esta habilitada, se muestra en pantalla, sino no.
////----------------aca esta la cosa
 Pic_Layers[Layer].Actual_Blink=Pic->Blink_Period*2; 	//refresca el periodo de blinkl por si esta en modo blink para que se mantenga encendida un periodo completo a partir de ahora..
//----------------------
 Layer_Info_Modified();
}
void Hide_Pic			(struct Struct_Pic* Pic)			
{
 unsigned char Layer=Search_Pic_Pos(Pic);
 Pic_Layers[Layer].Blink_State&=~0x01;
 Layer_Info_Modified();
}
void Enable_Periodic_Pic		(struct Struct_Pic* Pic)			
{
 unsigned char Layer=Search_Pic_Pos(Pic);
 Pic_Layers[Layer].Blink_State|=0x08;	//se procesa el cambio de estado entre hide/unhide  para hacer el blink
 Layer_Info_Modified();
}
void Disable_Periodic_Pic		(struct Struct_Pic* Pic)			
{
 unsigned char Layer=Search_Pic_Pos(Pic);
 Pic_Layers[Layer].Blink_State&=~0x08;
 Layer_Info_Modified();
}

void Add_Pic_On_Top_Index	(struct Struct_Pic* Pic,unsigned char Index)	{Set_Pic_In_Hole_Index(Pic_Layers_Used,Pic,Index);} 
void Add_Pic_On_Top		(struct Struct_Pic* Pic)			{Set_Pic_In_Hole(Pic_Layers_Used,Pic);} 
void Add_Pic_On_Bottom_Index	(struct Struct_Pic* Pic,unsigned char Index)	{Set_Pic_In_Hole_Index(0,Pic,Index);} 
void Add_Pic_On_Bottom		(struct Struct_Pic* Pic)			{Set_Pic_In_Hole(0,Pic);} 
void Add_Pic_On_Layer		(struct Struct_Pic* Pic,unsigned char Layer)	{Set_Pic_In_Hole(Layer,Pic);} 
void Del_Pic			(struct Struct_Pic* Pic)			{Cap_Hole(Search_Pic_Pos(Pic));}
void Del_All_Layers		(void)						{Pic_Layers_Used=0;Layer_Structure_Modified();}
void Layer_Structure_Modified	(void)						
{
	if( Layer_Modified!=0x02) {
		Send_NVDebug_Display_Layers_Data2Serial(10,"Modif->2\r\n");
		Atomic_Send_Event(Structure_Modified_Event,Display_Layers());
		Layer_Modified=0x02;
		//ppor alguna extrania razon, encuentro que sale por el serial el modif-2? pero luego NO llega el evento!!! pero a veces si.. de hecho la mayoria de las veces llega.. solo se pianta uno... y se cae todo.. no se porque...
	}
}
void Layer_Info_Modified	(void)	{
	if(!Layer_Modified) {
		Layer_Modified=0x01;
		Send_NVDebug_Display_Layers_Data2Serial(10,"Modif->1\r\n");
		Atomic_Send_Event(Info_Modified_Event,Display_Layers());
	}
}
void Does_Layer_Modified	(void)	{
	if( Layer_Modified)	
		Atomic_Send_Event(Info_Modified_Event,Display_Layers());
}
unsigned char Layer_Used	(void)						{return Pic_Layers_Used;}
unsigned char Read_Handler_Layer(void)						{return Handler_Layer;}
//-------------------------------------------------------------------------------------
void Print_Layers_Used		(void)						{Send_Char_NLine2Serial(Layer_Used());}
void Print_Actual_Layers	(void)						{Send_Char_NLine2Serial(Actual_Layer);}
void Print_Layer_Modified	(void)						{Send_Char_NLine2Serial(Layer_Modified);}
//-------------------------------------------------------------------------------------
State**  	Display_Layers		(void) 			{return &Display_Layers_Sm;} 			//devuelve la direccion de la maquina de estados Everythings para poder mandarle mensajes.
void 		Display_Layers_Rti	(void)			{Atomic_Send_Event(ANY_Event,Display_Layers());}	
void 		Init_Display_Layers	(void) 	
{
 Pic_Layers_Used=0;
 Display_Layers_Sm=Off;
 Layer_Modified=0;
 New_Periodic_Func_Schedule(5,Blink);
 New_Periodic_Func_Schedule(5,HScroll);
 New_None_Periodic_Schedule(3,Display_On_Event,Display_Layers());
 New_Periodic_Func_Schedule(4,Display_Layers_Rti);
}
//-------------------------------------------------------------------------------------
void Next_Layer2Display		(void)
{
	for(;Actual_Layer<Pic_Layers_Used && (Pic_Layers[Actual_Layer].Blink_State&0x03)!=0x03;Actual_Layer++) //salteo los que estan deshabilitados.
		;	
	if(Actual_Layer<Pic_Layers_Used) {
		Layer2Bank(&Pic_Layers[Actual_Layer]);
		Actual_Layer++;
//		Atomic_Send_Event(ANY_Event,Display_Layers());
		Send_NVDebug_Display_Layers_Data2Serial(10,"Next Disp ");
		Send_NVDebug_Display_Layers_Char_NLine2Serial(Actual_Layer);

	}
	else {
		Atomic_Send_Event(All_Updated_Event,Display_Layers());
		Send_NVDebug_Display_Layers_Data2Serial(11,"All Updat\r\n");
	}
}
//-------------------------------------------------------------------------------------
void Init_Actual_Layer					(void)	
{
	Actual_Layer=0;
	Layer_Modified=0;
	Send_NVDebug_Display_Layers_Data2Serial(10,"Modif->0\r\n");
}
void All_Displayed					(void)	{Atomic_Send_Event(All_Displayed_Event,Display_Layers());}
void Write_Disp_Bank_And_Does_Layer_Modified		(void)	
{
	Write_Disp_Bank();
	Does_Layer_Modified();
	Send_NVDebug_Display_Layers_Data2Serial(10,"Does Mod? ");
	Send_NVDebug_Display_Layers_Char_NLine2Serial(Layer_Modified);
}
void Init_Display_Phisical_And_Does_Layer_Modified	(void)	
{
	Init_Display_Phisical();
	Init_Display_Pics();
	Does_Layer_Modified();
}
//-------------------------------------------------------------------------------------
void Init_Actual_Layer_And_Clear_Bank			(void)	{Init_Actual_Layer();Clear_Bank();}


void Info_Modified_Idle(void)	{
	Send_NVDebug_Display_Layers_Data2Serial(11,"Info Idle\r\n");
	Init_Actual_Layer_And_Clear_Bank();
}
void Structure_Modified_Idle(void)	{
	Send_NVDebug_Display_Layers_Data2Serial(11,"Stru Idle\r\n");
	Init_Actual_Layer_And_Clear_Bank();
}
void Info_Modified_Updating(void)	{
	Send_NVDebug_Display_Layers_Data2Serial(11,"Info Upda\r\n");
}
void Structure_Modified_Updating(void)	{
	Send_NVDebug_Display_Layers_Data2Serial(11,"Stru Upda\r\n");
	Init_Actual_Layer_And_Clear_Bank();
}
void Print_Rti_Idle(void)	{
	Send_NVDebug_Display_Layers_Data2Serial(11,"Rti Idle \r\n");
	Does_Layer_Modified(); //NO seria necesario esto pero no encuentro donde esta el problema aun , que la maquina queda en idle con layer_modified en 2!! y eso seria imposible..
//lo causa el hecho de que se me pierde un evento.. no se como ...pero se pierde..
}
//-------------------------------------------------------------------------------------
static State Off [] RODATA =
{
{ Display_On_Event		,Init_Display_Phisical_And_Does_Layer_Modified	,Idle},
{ ANY_Event			,Rien						,Off},
};
static State Idle [] RODATA =
{
{ Info_Modified_Event		,Info_Modified_Idle				,Updating},
{ Structure_Modified_Event	,Structure_Modified_Idle			,Updating},
{ ANY_Event			,Print_Rti_Idle					,Idle},
};
static State Updating [] RODATA =
{
{ All_Updated_Event		,Write_Disp_Bank_And_Does_Layer_Modified	,Idle},
{ Info_Modified_Event		,Info_Modified_Updating				,Updating},
{ Structure_Modified_Event	,Structure_Modified_Updating			,Updating},
{ ANY_Event			,Next_Layer2Display				,Updating},
};
//-------------------------------------------------------------------------------------


