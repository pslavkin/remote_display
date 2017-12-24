#include "state_machine.h"
#include "events.h"
#include "display_phisical.h"
#include "display_layers.h"
#include "schedule.h"
#include "serial_tx.h"
#include "serial_session.h"
#include "str.h"
#include "type_conversion.h"
#include "buttons.h"
#include "everythings.h"
#include "setting_param_pic.h"
#include "font_3x4_pic.h"
#include "rotary.h"
#include "display_pics.h"

static unsigned char Lines[3][16]; 				//muestra 2 caracteres con font_3x4
struct Struct_Setting_Int_Param  Int_Param;
struct Struct_Setting_Char_Param Char_Param;
//--------------Pics Data----------------------------------
struct Struct_String_List Setting_Param_Data_Const[] RODATA=
{
{	0,Lines[0],		{Rien			,Rien			,Rien}},
{	0,Lines[1],		{Rien			,Rien			,Rien}},
{	0,Lines[2],		{Rien			,Rien			,Rien}},
};
//---------------Functions---------------------------------------
void Setting_Param_Constr(void)	
{
	Init_Font_3x4(); 
}
void Setting_Param_Destr(void)	
{
}
//----------CHAR-----------------------------------------------------------------------------------------
void 					Inc_Setting_Char_Param		(void)		
{
	if((*Char_Param.Value)<Char_Param.Max) (*Char_Param.Value)++;
	Char_Param.Update();
}
void 					Dec_Setting_Char_Param		(void)		
{
	if((*Char_Param.Value)>Char_Param.Min) (*Char_Param.Value)--;
	Char_Param.Update();
}
struct Struct_Setting_Char_Param* 	Read_Char_Setting_Params	(void)
{
	return &Char_Param;
}
void 					(*Read_Setting_Char_Params_Func	(void))(void)	
{
	return Char_Param.Return;
}
//}
void 					Pasive_Setting_Char_Param 	(void)	
{
	Pasive_Pic 	(&Setting_Char_Param_Pic);
	Dont_Blink_Pic	(&Setting_Char_Param_Pic);
}
void 					Active_Setting_Char_Param 	(void)	
{
	Active_Pic 	(&Setting_Char_Param_Pic);
	Do_Blink_Pic	(&Setting_Char_Param_Pic);
}
void 					Setting_Char_Param_Ok		(void)	
{
	Pasive_Setting_Char_Param();
	Char_Param.Return();
}
void 					Do_Blink_Setting_Char_Param 	(void)	
{
	Do_Blink_Pic	(&Setting_Char_Param_Pic);
}
void 					Dont_Blink_Setting_Char_Param	(void)
{
	Dont_Blink_Pic 	(&Setting_Char_Param_Pic);
}
void 					Hide_Setting_Char_Param	(void)
{
	Hide_Pic 	(&Setting_Char_Param_Pic);
}
void 					Unhide_Setting_Char_Param	(void)
{
	Unhide_Pic 	(&Setting_Char_Param_Pic);
}
void 					Setting_Char_Param_Update	(void)
{
	Char_Param.Update();
}
void 					Set_Setting_Char_Param		(struct Struct_Setting_Char_Param *Params)
{
	Char_Param=*Params;
	Char_Param.Update();
}
void 					Update_Setting_Char_Param	(void)
{
	unsigned char Buf[3];
	Char2Bcd(Buf,*Char_Param.Value);
	if(Buf[0]=='0') {
		Buf[0]='/';					//la barra es un espacio
		if(Buf[1]=='0') Buf[1]='/';			//si la centena es cero pregunto por la decena....
	}
	String2Pic(Buf,3,&Setting_Char_Param_Pic,&Font_3x4,0,0);
	Unhide_Pic(&Setting_Char_Param_Pic);
}
void 					Update_Setting_Charx10_Param	(void)
{
	unsigned char Buf[4];
	Char2Bcd(Buf,*Char_Param.Value);
	Buf[3]='0';
	Replace_Zero2Space(Buf,3);
	String2Pic(Buf+1,3,&Setting_Char_Param_Pic,&Font_3x4,0,0);
	Unhide_Pic(&Setting_Char_Param_Pic);
}
//----------INT-----------------------------------------------------------------------------------------
void 					Inc_Setting_Int_Param		(void)		
{
	if((*Int_Param.Value+Int_Param.Inc)<Int_Param.Max) 
		(*Int_Param.Value)+=Int_Param.Inc;
	else
		(*Int_Param.Value)=Int_Param.Max;

	Int_Param.Update();
}
void 					Dec_Setting_Int_Param		(void)		
{
	if((*Int_Param.Value)>Int_Param.Min+Int_Param.Inc) 
		(*Int_Param.Value)-=Int_Param.Inc;
	else
		(*Int_Param.Value)=Int_Param.Min;
	Int_Param.Update();
}
//------------------------------
struct Struct_Setting_Int_Param* 	Read_Setting_Int_Params		(void)	
{
	return &Int_Param;
}
void 					(*Read_Setting_Int_Params_Func	(void))(void)	
{
	return Int_Param.Return;
}
void 					Pasive_Setting_Int_Param 	(void)	
{
	Pasive_Pic 	(&Setting_Int_Param_Pic);
	Dont_Blink_Pic	(&Setting_Int_Param_Pic);
}
void 					Active_Setting_Int_Param 	(void)	
{
	Active_Pic 	(&Setting_Int_Param_Pic);
	Do_Blink_Pic	(&Setting_Int_Param_Pic);
}
void 					Setting_Int_Param_Ok		(void)	
{
	Pasive_Setting_Int_Param();
	Int_Param.Return();
}
void 					Do_Blink_Setting_Int_Param 	(void)	
{
	Do_Blink_Pic	(&Setting_Int_Param_Pic);
}
void 					Dont_Blink_Setting_Int_Param	(void)
{
	Dont_Blink_Pic 	(&Setting_Int_Param_Pic);
}
void 					Setting_Int_Param_Update	(void)
{
	Int_Param.Update();
}
void 					Set_Setting_Int_Param		(struct Struct_Setting_Int_Param *Params)
{
	Int_Param=*Params;
	Int_Param.Update();
}
void 					Update_Setting_Int_Param	(void)
{
	unsigned char Buf[4];
	Int2_4Digit_Bcd(Buf,*Int_Param.Value);
	Replace_Zero2Space(Buf,3);					//ceros a la izquierda out
	String2Pic(Buf,4,&Setting_Int_Param_Pic,&Font_3x4,0,0);
	Unhide_Pic(&Setting_Int_Param_Pic);
}
//--------------Events----------------------------------
struct Struct_Pic_Events Setting_Char_Param_Events[] RODATA =
{		
{ Invalid_Button_Code,		{Setting_Param_Constr,		Rien,		Setting_Param_Destr}},
{ Rotary_ACW_Code,		{Dec_Setting_Char_Param,	Rien,		Rien}},
{ Rotary_CW_Code,		{Inc_Setting_Char_Param,	Rien,		Rien}},
{ Button_Rotary_Code,		{Rien,				Rien,		Setting_Char_Param_Ok}},
};
struct Struct_Pic_Events Setting_Int_Param_Events[] RODATA =
{		
{ Invalid_Button_Code,		{Setting_Param_Constr,		Rien,		Setting_Param_Destr}},
{ Rotary_ACW_Code,		{Dec_Setting_Int_Param,		Rien,		Rien}},
{ Rotary_CW_Code,		{Inc_Setting_Int_Param,		Rien,		Rien}},
{ Button_Rotary_Code,		{Rien,				Rien,		Setting_Int_Param_Ok}},
};
//--------------Pics Info----------------------------------
struct Struct_Pic Setting_Char_Param_Pic RODATA	={9 ,1,20,4,1,0,4,Setting_Char_Param_Events	,{Setting_Param_Data_Const}};
struct Struct_Pic Setting_Int_Param_Pic	 RODATA	={5 ,1,20,4,1,0,4,Setting_Int_Param_Events 	,{Setting_Param_Data_Const}};
//------------------------------------------------------
void 				Add_Setting_Char_Param		(void)	{Add_Pic_On_Top 	(&Setting_Char_Param_Pic);}
void 				Del_Setting_Char_Param		(void)	{Del_Pic 		(&Setting_Char_Param_Pic);}
void 				Add_Setting_Int_Param		(void)	{Add_Pic_On_Top 	(&Setting_Int_Param_Pic);}
void 				Del_Setting_Int_Param		(void)	{Del_Pic 		(&Setting_Int_Param_Pic);}
//------------------------------------------------------
