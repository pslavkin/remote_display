#include <display_phisical.h>
#include <display_pics.h>
#include <display_layers.h>
#include <buttons.h>
#include <rotary.h>
#include <setting_menu_pic.h>
#include <setting_bank_pic.h>
#include <power_pic.h>
#include <blank_pic.h>
#include <time_pic.h>
#include <programs_pic.h>
#include <font_slide_1x1_pic.h>
#include <font_3x4_pic.h>
#include <memory_pic.h>
#include <serial_tx.h>
#include <type_conversion.h>

unsigned char Selected_Bank;	
//----------------PROGRAMS----------------------------------------------------------------------------
unsigned char Customs_Chars_Programs[][8] RODATA =	{
	{0x00,0x00,0x00,0x00,0x01,0x02,0x14,0x08},
	{0x00,0x00,0x00,0x00,0x1F,0x1F,0x1F,0x1F},
	{0x00,0x00,0x00,0x1F,0x1F,0x1F,0x1F,0x1F},
	{0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F},
	};
void Init_Custom_Chars_Programs		(void) 	
{
	Write_Custom_Bank(sizeof(Customs_Chars_Programs),(unsigned char*)Customs_Chars_Programs[0]);
}
//-------------MEMORY---------------------------------------------------------------------------------
unsigned char Customs_Chars_Memory[][8] RODATA =	{
	{0x00,0x00,0x00,0x00,0x03,0x07,0x0F,0x1F}, 
	{0x00,0x00,0x00,0x00,0x1F,0x1F,0x1F,0x1F}, 
	{0x01,0x03,0x07,0x0F,0x1F,0x1F,0x1F,0x1F}, 
	{0x0E,0x0E,0x0E,0x0E,0x00,0x00,0x00,0x00}, 
	{0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F}, 
	};
void Init_Custom_Chars_Memory		(void) 	
{
	Write_Custom_Bank(sizeof(Customs_Chars_Memory),Customs_Chars_Memory[0]);
}
//------------------BANK-----------------------------------------------------------------------------
unsigned char Customs_Chars_Setting_Bank[][8] RODATA =	{
	{0x00,0x00,0x00,0x00,0x1F,0x1F,0x1F,0x1F}, 
	{0x1F,0x1F,0x1F,0x1F,0x00,0x00,0x00,0x00}, 
	{0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F}, 
	};
void Init_Custom_Chars_Setting_Bank		(void) 	
{
	Write_Custom_Bank(sizeof(Customs_Chars_Setting_Bank),Customs_Chars_Setting_Bank[0]);
}
//--------------Pics Data----------------------------------
struct Struct_String_List Setting_Bank_Data_Const[] RODATA =
{
{	0,(uint8_t*)" Programas  "					,{Setting_Programs_Down	,Setting_Programs_Up	,Select_Programs}},
{	0,(uint8_t*)"\x20\x20\x09\x09\x09\x20\x20\x09\x09\x09\x20\x20"	,{Rien			,Rien			,Rien}},
{	0,(uint8_t*)"\x20\x20\x0B\x08\x0B\x20\x20\x0B\x08\x0B\x20\x20"	,{Rien			,Rien			,Rien}},
{	0,(uint8_t*)"\x20\x20\x0B\x0A\x0B\x20\x20\x0B\x0A\x0B\x20\x20"	,{Rien			,Rien			,Rien}},

{	0,(uint8_t*)"  Memoria   "					,{Memory_Down		,Memory_Up		,Select_Memory}},
{	0,(uint8_t*)"\x20\x20\x20\x08\x09\x09\x09\x09\x09\x20\x20\x20"	,{Rien			,Rien			,Rien}},
{	0,(uint8_t*)"\x20\x20\x0A\x20\x0B\x0B\x0B\x0B\x0C\x20\x20\x20"	,{Rien			,Rien			,Rien}},
{	0,(uint8_t*)"\x20\x20\x0C\x09\x09\x09\x09\x09\x0C\x20\x20\x20"	,{Rien			,Rien			,Rien}},

{	0,(uint8_t*)"   Banco A  "			    		,{Select_A_Down		,Select_A_Up		,Select_A}},
{	0,(uint8_t*)"\x20\x20\x20\x20\x08\x08\x08\x08\x20\x20\x20\x20"	,{Rien			,Rien			,Rien}},
{	0,(uint8_t*)"\x20\x20\x20\x0A\x08\x08\x08\x08\x0A\x20\x20\x20"	,{Rien			,Rien			,Rien}},
{	0,(uint8_t*)"\x20\x20\x20\x0A\x20\x20\x20\x20\x0A\x20\x20\x20"	,{Rien			,Rien			,Rien}},

{	0,(uint8_t*)"   Banco B  "		    			,{Select_B_Down		,Select_B_Up		,Select_B}},
{	0,(uint8_t*)"\x20\x20\x20\x08\x08\x08\x08\x08\x20\x20\x20\x20"	,{Rien			,Rien			,Rien}},
{	0,(uint8_t*)"\x20\x20\x20\x0A\x08\x08\x08\x08\x0A\x20\x20\x20"	,{Rien			,Rien			,Rien}},
{	0,(uint8_t*)"\x20\x20\x20\x0A\x08\x08\x08\x08\x09\x20\x20\x20"	,{Rien			,Rien			,Rien}},
};
//---------------Functions---------------------------------------
void Setting_Bank_Constr(void)
{
 	Init_Custom_Chars_Programs();
}
void Setting_Bank_Destr	(void)
{
	Init_Font_Slide_1x1();
	Init_Font_3x4(); 
	Set_Time_Param();
	Del_Blank();
}
void Setting_Programs_Up	(void)	{}
void Setting_Programs_Down	(void)	{Init_Custom_Chars_Memory();		V4Scroll2Down();}
void Memory_Up			(void)	{Init_Custom_Chars_Programs();		V4Scroll2Up();}
void Memory_Down		(void)	{Init_Custom_Chars_Setting_Bank();	V4Scroll2Down();}
void Select_A_Up		(void)	{Init_Custom_Chars_Memory();		V4Scroll2Up();}
void Select_A_Down		(void)	{					V4Scroll2Down();}
void Select_B_Up		(void)	{					V4Scroll2Up();}
void Select_B_Down		(void)	{}
void Disable_Setting_Bank	(void) 	{Disable_Pic(&Setting_Bank_Pic);Pasive_Pic(&Setting_Bank_Pic);}
void Enable_Setting_Bank	(void) 	{Enable_Pic (&Setting_Bank_Pic);Active_Pic(&Setting_Bank_Pic);}
void Select_Programs		(void)
{
	Add_Programs();
}
void Select_Memory		(void)
{
	Add_Memory();
}
void Select_A			(void)
{
	Set_Bank_Changing(0);	
	Add_Setting_Menu();
}
void Select_B			(void)
{
	Set_Bank_Changing(1);	
	Add_Setting_Menu();
}
//--------------Events----------------------------------
struct Struct_Pic_Events Setting_Bank_Events[] RODATA =
{		
{ Invalid_Button_Code,		{Setting_Bank_Constr		,Rien			,Setting_Bank_Destr}},
{ Rotary_ACW_Code,		{String_List_Handler0		,Rien			,Rien}},
{ Rotary_CW_Code,		{String_List_Handler1		,Rien			,Rien}},
{ Button_Rotary_Code,		{Rien				,Del_Setting_Bank	,String_List_Handler2}},
};
//--------------Pics Info----------------------------------
struct Struct_Pic Setting_Bank_Pic RODATA ={4,0,16,4,0,0,4,Setting_Bank_Events,{Setting_Bank_Data_Const}};
//------------------------------------------------------
void Add_Setting_Bank	(void)	{Add_Blank();	Add_Pic_On_Top 	(&Setting_Bank_Pic);}
void Del_Setting_Bank	(void)	{		Del_Pic 	(&Setting_Bank_Pic);}
//------------------------------------------------------
