#include <state_machine.h>
#include <events.h>
#include <display_phisical.h>
#include <display_pics.h>
#include <display_layers.h>
#include <schedule.h>
#include <serial_tx.h>
#include <serial_session.h>
#include <string.h>
#include <type_conversion.h>
#include <buttons.h>
#include <everythings.h>
#include <setting_param_pic.h>
#include <font_3x3_pic.h>

//--------------Pics Data----------------------------------
unsigned char Customs_Chars[][8] RODATA=	
{
	{0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F},
};

struct Struct_String_List Sel_Arrows_Data[] RODATA=
{
{ 0,(uint8_t*)"\x00",		{Rien,Rien,Rien}},
{ 0,(uint8_t*)" ",		{Rien,Rien,Rien}},
{ 0,(uint8_t*)"",		{Rien,Rien,Rien}},
};
//---------------Functions---------------------------------------
//--------------Events----------------------------------
//--------------Pics Info----------------------------------
struct Struct_Pic Sel_Arrows_Pic RODATA=		{0,0,0,0,0,0,1,Rien_Events,{Sel_Arrows_Data}};
//------------------------------------------------------
void Init_Customs_Chars_Arrows			(void) 	
{
	Write_Custom_Char(sizeof(Customs_Chars),(uint8_t*)Customs_Chars[0]);
}
void Add_Sel_Arrows 			(void)	{Add_Pic_On_Top	(&Sel_Arrows_Pic);}
void Del_Sel_Arrows 			(void)	{Del_Pic	(&Sel_Arrows_Pic);}
//------------------------------------------------------

