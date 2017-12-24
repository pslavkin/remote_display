#include "display_layers.h"
#include "display_pics.h"
#include "buttons.h"
#include "rotary.h"
#include "buzzer.h"
#include "player_pic.h"
#include "time_pic.h"
#include "power_pic.h"
#include "pointer_pic.h"
#include "everythings.h"
#include "type_conversion.h"
#include "leds_session.h"
#include "estim_phisical.h"

//--------------Pics Data----------------------------------
struct Struct_String_List Player_Data_Const[] RODATA=
{
	{0,(uint8_t*)"Inactivo"		,{Rien		,Rien	,Player_Active}},
	{0,(uint8_t*)"  Activo"    	,{Dec_Time	,Rien	,Player_Inactive}},
};
//---------------Functions---------------------------------------
void Player_Active(void)
{
	if(Read_Time()>0){
		Set_Led_Effect(Led_Red_Index,	0xFFFF);
		Set_Led_Effect(Led_Green_Index,	0x0000);
		Set_Led_Effect(Led_Blue_Index,	0x0000);
		Refresh_Pic_Blink((struct Struct_Pic*)&Player_Pic);			//el tiempo inicial de la pic podria estar en cualqiuer parte y lo uso para llevar el tiempo del tratamiento...
		V1Scroll2Down_Pic((struct Struct_Pic*)&Player_Pic);
		Beeeeeep();	
		Turn_On_Estim_Phisical();	//arranca la secuencia..
	}
}
void Player_Inactive(void)
{
	Set_Led_Effect(Led_Red_Index,	0x0000);
	Set_Led_Effect(Led_Green_Index,	0xFFFF);
	Set_Led_Effect(Led_Blue_Index,	0x0000);
	V1Scroll2Up_Pic((struct Struct_Pic*)&Player_Pic);
	Beep_Beep_Beeeep();
	Turn_Off_Estim_Phisical();	//termina la secuencia donde sea que este..
}

void Move_Pointer2Player(void)
{
	Set_Pointer_RArrow();
	Move_Pointer(11,0);
}
void Player_Constr(void)	
{
}
void Player_Destr(void)	
{
}
void Toogle_Player_State	(void)	
{
	Pic_String_List_Handler2(&Player_Pic);
}
//--------------Events----------------------------------
struct Struct_Pic_Events Player_Events[] RODATA =
{		
{ Invalid_Button_Code,		{Player_Constr		,String_List_Handler0			,Player_Destr}},
};
//--------------Pics Info----------------------------------
struct Struct_Pic Player_Pic RODATA ={12,0,20,1,2*60,0,1,Player_Events,{Player_Data_Const}};
//------------------------------------------------------
void Add_Player	(void)	{Add_Pic_On_Top	(&Player_Pic);}
void Del_Player	(void)	{Del_Pic 	(&Player_Pic);}
//------------------------------------------------------
