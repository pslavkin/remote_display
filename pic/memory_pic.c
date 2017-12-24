#include "display_phisical.h"
#include "display_layers.h"
#include "display_pics.h"
#include "setting_param_pic.h"
#include "buttons.h"
#include "rotary.h"
#include "blank_pic.h"
#include "memory_pic.h"
#include "setting_bank_pic.h"
#include "programs_pic.h"
#include "type_conversion.h"

static unsigned char Actual_Program;
//----------------------------------------------------------------------------------------------------
void Setting_Eeprom_Num(void)
{
 struct Struct_Setting_Char_Param Param={1,5,1,(unsigned char*)0,Update_Setting_Char_Param,Rien};
 Actual_Program=1;
 Param.Value=&Actual_Program;
 Set_Setting_Char_Param(&Param);
}
//--------------Pics Data----------------------------------
struct Struct_String_List Memory_Data_Const[] RODATA=
{
	{0,(uint8_t*)"1-Personalizado 1   "	,{Rien		,Rien		,Save_Eeprom_Program}},
	{0,(uint8_t*)"2-Personalizado 2   "	,{Rien		,Rien		,Save_Eeprom_Program}},
	{0,(uint8_t*)"3-Personalizado 3   "	,{Rien		,Rien		,Save_Eeprom_Program}},
	{0,(uint8_t*)"4-Personalizado 4   "	,{Rien		,Rien		,Save_Eeprom_Program}},
	{0,(uint8_t*)"5-Personalizado 5   "	,{Rien		,Rien		,Save_Eeprom_Program}},
	{0,(uint8_t*)""			 	,{Rien		,Rien		,Rien}},
};
//---------------Functions---------------------------------------
void Memory_Constr(void)
{
	Add_Setting_Char_Param();
	Pasive_Setting_Char_Param();
	Setting_Eeprom_Num();
}
void Memory_Destr	(void)
{
	Del_Setting_Char_Param();
	Del_Blank();
	Init_Custom_Chars_Memory(); 	
}
void Save_Eeprom_Program		(void)  
{
	Save_Banks2Eeprom_Program(Actual_Program-1); 
	Del_Memory();
}
//--------------Events----------------------------------
struct Struct_Pic_Events Memory_Events[] RODATA=
{		
{ Invalid_Button_Code,		{Memory_Constr				,Rien			,Memory_Destr}},
{ Rotary_ACW_Code,		{V1Scroll2Up_And_Dec_Setting_Param	,Rien			,Rien}},
{ Rotary_CW_Code,		{V1Scroll2Down_And_Inc_Setting_Param	,Rien			,Rien}},
{ Button_Rotary_Code,		{Rien					,Del_Memory		,String_List_Handler2}},
};
//--------------Pics Info----------------------------------
struct Struct_Pic Memory_Pic RODATA= {0,0,20,1,0,0,4,Memory_Events,{Memory_Data_Const}};
//------------------------------------------------------
void Add_Memory	(void)	{Add_Blank();	Add_Pic_On_Top	(&Memory_Pic);}
void Del_Memory	(void)	{		Del_Pic 	(&Memory_Pic);}
//------------------------------------------------------
