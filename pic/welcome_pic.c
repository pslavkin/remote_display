#include "display_pics.h"
#include "display_layers.h"
#include "buttons.h"
#include "welcome_pic.h"
#include "power_pic.h"
#include "stdint.h"
#include "type_conversion.h"
#include "debug.h"
#include "serial_tx.h"

//--------------Pics Data----------------------------------
struct Struct_String_List Welcome_Data_Const [] RODATA= 
{
{	0,(uint8_t*)" Electroestimulador ",{		Del_Welcome		,Rien			,Rien}},
{	0,(uint8_t*)"   --- N1108 ---    ",{		Rien			,Rien			,Rien}},
{	0,(uint8_t*)"                    ",{		Rien			,Rien			,Rien}},
{	0,(uint8_t*)"   gruponoto.com    ",{		Rien			,Rien			,Rien}},
};
//---------------Functions---------------------------------------
void Welcome_Constr(void)	
{
 	Send_NVDebug_Welcome_Pic_Data2Serial(16,"Welcome Constr\r\n");
}
void Welcome_Destr(void)	
{
 	Send_NVDebug_Welcome_Pic_Data2Serial(16,"Welcome Destr\r\n");
	Add_Power();
}
//--------------Events----------------------------------
struct Struct_Pic_Events Welcome_Events[] RODATA =
{		
{ Invalid_Button_Code,		{Welcome_Constr,		String_List_Handler0,		Welcome_Destr}},
{ Any_Button_Code,		{Rien,				Rien,				String_List_Handler0}},
};
//--------------Pics Info----------------------------------
struct Struct_Pic Welcome_Pic RODATA=
{
	0,0,20,4,10,0,2,Welcome_Events,{Welcome_Data_Const}
};
//------------------------------------------------------
void Add_Welcome	(void)	{Add_Pic_On_Top 	(&Welcome_Pic);}
void Del_Welcome	(void)	{Del_Pic		(&Welcome_Pic);}
//------------------------------------------------------
