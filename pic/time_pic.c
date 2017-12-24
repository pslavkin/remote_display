#include <display_phisical.h>
#include <display_layers.h>
#include <time_pic.h>
#include <setting_param_pic.h>
#include <pointer_pic.h>
#include <power_pic.h>
#include <buzzer.h>
#include <schedule.h>
#include <player_pic.h>

unsigned char Time;
//--------------Pics Data------------------------------
//---------------Functions-----------------------------
void Move_Pointer2Time(void)
{
 Move_Pointer(12,2);
 Set_Pointer_RArrow();
}
void Set_Time_Param(void)
{ 
 	struct Struct_Setting_Char_Param Param={1,20,1,(unsigned char*)0,Update_Setting_Char_Param,Active_Power};
 	Param.Value=&Time;
 	Set_Setting_Char_Param(&Param);
}
void 		Active_Time_Param	(void)	{Active_Setting_Char_Param();}
unsigned char 	Read_Time		(void)	{return Time;}
void 		Dec_Time		(void)
{
	if(!--Time) Toogle_Player_State();
	if(Read_Setting_Char_Params_Func()==Active_Power) Setting_Char_Param_Update();
}
//--------------Events----------------------------------
//--------------Pics Info-------------------------------
void Add_Time	(void)	
{	
	Time=10;
	Add_Setting_Char_Param();
	Set_Time_Param();
	Pasive_Setting_Char_Param();
}	
void Del_Time	(void)	
{
	Del_Setting_Char_Param();
}
//------------------------------------------------------
