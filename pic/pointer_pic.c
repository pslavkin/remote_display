#include "display_phisical.h"
#include "display_layers.h"
#include "display_pics.h"
#include "buttons.h"
#include "rotary.h"
#include "pointer_pic.h"
#include "type_conversion.h"

//--------------Pics Data----------------------------------
struct Struct_String_List Pointer_Data_Const[] RODATA =
{	
//ojo con los modelos de displays que no todos tienen implementado el mismo mapa de caracteres por fuera de los ascii...
{ 0,(uint8_t*)"x"/*"\x7F"*/,		{Rien,Rien,Rien}},		//flechas a izq. 	
{ 0,(uint8_t*)"x"/*"\x7F"*/,		{Rien,Rien,Rien}},		//flechas a izq.
{ 0,(uint8_t*)"x"/*"\x7F"*/,		{Rien,Rien,Rien}},		//flechas a izq.
{ 0,(uint8_t*)"x"/*"\x7F"*/,		{Rien,Rien,Rien}},		//flechas a izq.
{ 0,(uint8_t*)"x"/*"\x7E"*/,		{Rien,Rien,Rien}},		//flecha a der.
};
//---------------Functions---------------------------------------
void Pointer_Constr(void)	
{
}
void Pointer_Destr(void)	
{
}
void 		Disable_Pointer		(void) 	{Disable_Pic	(&Pointer_Pic);Pasive_Pic(&Pointer_Pic);}
void 		Enable_Pointer		(void) 	{Enable_Pic 	(&Pointer_Pic);Active_Pic(&Pointer_Pic);}
void 		Blink_Pointer		(void) 	{Do_Blink_Pic	(&Pointer_Pic);}
void 		Dont_Blink_Pointer	(void) 	{Dont_Blink_Pic	(&Pointer_Pic);}

void Move_Pointer	(unsigned X,unsigned Y)	
{
	Pointer_Pic.StartX=X;
	Pointer_Pic.EndX=X+1;
	Pointer_Pic.StartY=Y;
	Pointer_Pic.EndY=Y+1;
	Layer_Info_Modified();
}	
void	Move_Pointer2Down				(void)
{
	Pointer_Pic.StartY++;
	Pointer_Pic.EndY++;
	Actual_Pic_Handler_Index2Down();
	Layer_Info_Modified();
}
void	Move_Pointer2Up					(void)
{
	Pointer_Pic.StartY--;
	Pointer_Pic.EndY--;
	Actual_Pic_Handler_Index2Up();
	Layer_Info_Modified();
}
void 	Set_Pointer_Heigh_1				(void)
{
	Pointer_Pic.EndY=Pointer_Pic.StartY+1;
}
void	Set_Pointer_Heigh_4				(void)
{
	Pointer_Pic.EndY=Pointer_Pic.StartY+4;
}
void	Set_Pointer_RArrow				(void)
{
	Set_Pic_Item_And_Handler_Index(&Pointer_Pic,4);
}
void	Set_Pointer_LArrow				(void)
{
	Set_Pic_Item_And_Handler_Index(&Pointer_Pic,0);
}
//--------------Events----------------------------------
struct Struct_Pic_Events Pointer_Events[] RODATA=
{		
{ Invalid_Button_Code,		{Pointer_Constr	,Rien	,Pointer_Destr}},
};
//--------------Pics Info----------------------------------
struct Struct_Pic Const_Pointer_Pic RODATA={0,0,1,1,1,0,1,Pointer_Events,{Pointer_Data_Const}};
struct Struct_Pic Pointer_Pic;
//------------------------------------------------------
void Add_Pointer	(void)		{Pointer_Pic=Const_Pointer_Pic;	Add_Pic_On_Top 	(&Pointer_Pic);}
void Del_Pointer	(void)		{				Del_Pic 	(&Pointer_Pic);}
//------------------------------------------------------
