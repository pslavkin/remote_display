#include "display_phisical.h"
#include "display_pics.h"
#include "display_layers.h"
#include "buttons.h"
#include "blank_pic.h"
#include "type_conversion.h"

//--------------Pics Data----------------------------------
struct Struct_String_List Blank_Data_Const[] RODATA= {
{	0, (uint8_t*)"                    ",{Rien,Rien,Rien}}, 
{	0, (uint8_t*)"                    ",{Rien,Rien,Rien}}, 
{	0, (uint8_t*)"                    ",{Rien,Rien,Rien}}, 
{	0, (uint8_t*)"                    ",{Rien,Rien,Rien}}, 
};
//---------------Functions---------------------------------------
void Blank_Constr(void)	
{
}
void Blank_Destr(void)	
{
}
void Blank_Any(void)	
{
}
//--------------Events----------------------------------
struct Struct_Pic_Events Blank_Events[] RODATA=
{		
{ Invalid_Button_Code,		{Blank_Constr,			Rien,				Blank_Destr}},
{ Any_Button_Code,		{Rien,				Rien,				Rien}},
};
//--------------Pics Info----------------------------------
struct Struct_Pic Blank_Pic RODATA=
{
	0,0,20,4,0,0,2,Blank_Events,{Blank_Data_Const}
};
//------------------------------------------------------
void Add_Blank	(void)	{Add_Pic_On_Top(&Blank_Pic);}
void Del_Blank	(void)	{Del_Pic(&Blank_Pic);}
//------------------------------------------------------
