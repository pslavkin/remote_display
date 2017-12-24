#include "display_phisical.h"
#include "display_layers.h"
#include "display_pics.h"
#include "str.h"
#include "type_conversion.h"
#include "buttons.h"
#include "rotary.h"
#include "everythings.h"
#include "power_pic.h"
#include "font_slide_1x1_pic.h"
#include "setting_bank_pic.h"
#include "setting_param_pic.h"
#include "pointer_pic.h"
#include "blank_pic.h"
#include "time_pic.h"
#include "player_pic.h"
#include "programs_pic.h"
#include "tpm.h"
#include "setting_menu_pic.h"
#include "debug.h"
#include "serial_tx.h"
#include "memory_pic.h"
#include "estim_phisical.h"
#include "buzzer.h"
#include "leds_session.h"

//--------------Pics Data----------------------------------
struct Struct_Power 		Power[MAX_CHANNELS];
static unsigned char		Lines[4][SLIDE_LENGTH+1]; 		//muestra 3 caracteres con font_1x1
struct Struct_String_List	Power_Data_Const[] RODATA=
{
{ 0,(uint8_t*)"",	{Move_Pointer2Powe_1_And_Actual_Pic_Handler_Index2Down	,Rien							,V7Scroll2Down_And_Blink_Pointer}},
{ 0,(uint8_t*)Lines[0],	{Move_Pointer2Down					,Move_Pointer2Power_All_And_Actual_Pic_Handler_Index2Up	,V7Scroll2Down_And_Blink_Pointer}},
{ 0,(uint8_t*)Lines[1],	{Move_Pointer2Down					,Move_Pointer2Up					,V7Scroll2Down_And_Blink_Pointer}},
{ 0,(uint8_t*)Lines[2],	{Move_Pointer2Down					,Move_Pointer2Up					,V7Scroll2Down_And_Blink_Pointer}},
{ 0,(uint8_t*)Lines[3],	{Move_Pointer2Time_And_Actual_Pic_Handler_Index2Down	,Move_Pointer2Up					,V7Scroll2Down_And_Blink_Pointer}},
{ 0,(uint8_t*)"",    	{Move_Pointer2Player_And_Actual_Pic_Handler_Index2Down	,Move_Pointer2Power_4_And_Actual_Pic_Handler_Index2Up	,Pasive_Power_And_Active_Time_Param}},
{ 0,(uint8_t*)"",    	{Rien							,Move_Pointer2Time_And_Actual_Pic_Handler_Index2Up	,Play_Stop}},

{ 0,(uint8_t*)"",	{Inc_Power_Ch_All					,Dec_Power_Ch_All					,V7Scroll2Up_And_Dont_Blink_Pointer}},
{ 0,(uint8_t*)Lines[0],	{Inc_Power_Ch0						,Dec_Power_Ch0						,V7Scroll2Up_And_Dont_Blink_Pointer}},
{ 0,(uint8_t*)Lines[1],	{Inc_Power_Ch1						,Dec_Power_Ch1						,V7Scroll2Up_And_Dont_Blink_Pointer}},
{ 0,(uint8_t*)Lines[2],	{Inc_Power_Ch2						,Dec_Power_Ch2						,V7Scroll2Up_And_Dont_Blink_Pointer}},
{ 0,(uint8_t*)Lines[3],	{Inc_Power_Ch3						,Dec_Power_Ch3						,V7Scroll2Up_And_Dont_Blink_Pointer}},
{ 0,(uint8_t*)"",	{Rien							,Rien							,Rien}},
{ 0,(uint8_t*)"",	{Rien							,Rien							,Rien}},
};
//---------------Functions---------------------------------------
void Power_Constr				(void)	
{
	unsigned char Ch;
	Init_Font_Slide_1x1();
	Assign_Tpm_Power_Func();
	for(Ch=0;Ch<MAX_CHANNELS;Ch++){
		Power[Ch].Wished=0;
 		Lines[Ch][0]=Ch+'1';
		Update_Power_Ch(Ch);
	}
	Set_Actual_Power_All(0);
 	Add_Pointer();
	Move_Pointer2Player();
	Set_Pic_Handler_Index(&Power_Pic,6);
	Set_Pic_Item_Index(&Power_Pic,1); 	//lo pongo en la posicion uno porque recien ahi esta la info que quiero mostrar..
 	Send_NVDebug_Power_Pic_Data2Serial(13,"Power Contr\r\n");
}
void Power_Destr				(void)	
{
	Del_Pointer();
	Del_Time();
	Del_Player();
 	Send_NVDebug_Power_Pic_Data2Serial(13,"Power Destr\r\n");
}
//----------------------------------------------------------------------------------------------------
void Assign_Tpm_Power_Func(void)
{
	Power[0].Tpm_Power=Set_Tpm2C1;
	Power[1].Tpm_Power=Set_Tpm2C0;
	Power[2].Tpm_Power=Set_Tpm2C3;
	Power[3].Tpm_Power=Set_Tpm2C2;
}
void Update_Rise_Step(unsigned char Ch)
{
	Power[Ch].Rise_Step	=((unsigned int)Power[Ch].Wished*512)/Read_A();
	Power[Ch].Acc_Step	=0;
 		Send_NVDebug_Power_Pic_Data2Serial(5,"Rise ");
		Send_NVDebug_Power_Pic_Int_NLine2Serial(Power[Ch].Rise_Step);
}
void Update_Rise_Step_All(void)
{
	unsigned char i;
	for(i=0;i<MAX_CHANNELS;i++) Update_Rise_Step(i);
}
void Update_Fall_Step(unsigned char Ch)
{
	Power[Ch].Fall_Step	=(Power[Ch].Acc_Step=(unsigned int)Power[Ch].Actual*512)/Read_E();
 		Send_NVDebug_Power_Pic_Data2Serial(5,"Fall ");
		Send_NVDebug_Power_Pic_Int_NLine2Serial(Power[Ch].Fall_Step);
}
void Update_Fall_Step_All(void)
{
	unsigned char i;
	for(i=0;i<MAX_CHANNELS;i++) Update_Fall_Step(i);
}
void Set_Actual_Power(unsigned char Ch, unsigned char APower)
{
	Power[Ch].Actual=APower;
	Power[Ch].Tpm_Power(APower);
	Set_Led_Effect(Led_Ch1_Index+Ch,APower?0xFFFF:0x00);
// 		Send_NVDebug_Power_Pic_Data2Serial(7,"APower ");
//		Send_NVDebug_Power_Pic_Char_NLine2Serial(APower);
}
void Set_Actual_Power_All(unsigned char Power)
{
	unsigned char i;
	for(i=0;i<MAX_CHANNELS;i++) Set_Actual_Power(i,Power);
}
void Wished_Power2Actual(void)
{
	unsigned char i,Enabled_Channels=Actual_Sequence_Data();
	for(i=0;i<MAX_CHANNELS;i++) 
		if(Enabled_Channels & (0x01<<i)) Set_Actual_Power(i,Power[i].Wished);
}
void Inc_Power_ChX				(unsigned char Ch)
{
	if(Power[Ch].Wished<100) Power[Ch].Wished++; else Beep();
	Update_Power_Ch(Ch);
}

void Dec_Power_ChX				(unsigned char Ch)
{
	if(Power[Ch].Wished>  0) Power[Ch].Wished--; else Beep();
	Update_Power_Ch(Ch);
}
void Inc_Power_Ch0				(void)	{Inc_Power_ChX(0);}
void Inc_Power_Ch1				(void)	{Inc_Power_ChX(1);}
void Inc_Power_Ch2				(void)	{Inc_Power_ChX(2);}
void Inc_Power_Ch3				(void)	{Inc_Power_ChX(3);}

void Dec_Power_Ch0				(void)	{Dec_Power_ChX(0);}
void Dec_Power_Ch1				(void)	{Dec_Power_ChX(1);}
void Dec_Power_Ch2				(void)	{Dec_Power_ChX(2);}
void Dec_Power_Ch3				(void)	{Dec_Power_ChX(3);}

void Inc_Power_Ch_All				(void)
{
	unsigned char Ch;
	for(Ch=0;Ch<MAX_CHANNELS;Ch++)	Inc_Power_ChX(Ch);
}
void Dec_Power_Ch_All				(void)
{
	unsigned char Ch;
	for(Ch=0;Ch<MAX_CHANNELS;Ch++)	Dec_Power_ChX(Ch);
}
void Ramp_Up_Power(unsigned char Ch)
{
	unsigned char Next=(Power[Ch].Acc_Step+=Power[Ch].Rise_Step)/512;
	if(Next<=Power[Ch].Wished) Set_Actual_Power(Ch,Next);
}
void Ramp_Up_Power_All(void)
{
	unsigned char i,Enabled_Channels=Actual_Sequence_Data();
	for(i=0;i<MAX_CHANNELS;i++) 
		if(Enabled_Channels & (0x01<<i)) Ramp_Up_Power(i);
}
void Ramp_Down_Power(unsigned char Ch)
{
	  signed char Next=(Power[Ch].Acc_Step-=Power[Ch].Fall_Step)/512;
	if(Next>=0) Set_Actual_Power(Ch,Next);
}
void Ramp_Down_Power_All(void)
{
	unsigned char i;
	for(i=0;i<MAX_CHANNELS;i++) Ramp_Down_Power(i);
}
//----------------------------------------------------------------------------------------------------
void Pasive_Power				(void) 	{Pasive_Pic(&Power_Pic);}
void Active_Power				(void) 	{Active_Pic(&Power_Pic);}

void Update_Power_Ch				(unsigned char Ch)
{
	Slide_Fill(&Lines[Ch][1],0x07,Power[Ch].Wished/10,'-',SLIDE_LENGTH);
	Layer_Info_Modified();
}
void Update_All_Power_Ch			(void)
{
	unsigned char Ch;
	for(Ch=0;Ch<MAX_CHANNELS;Ch++)	Update_Power_Ch(Ch);
}
static void Button_Rotary_Hold_Func		(void)
{
	Add_Setting_Bank();
}
void Move_Pointer2Power_1			(void)
{
	Move_Pointer(10,0);
	Set_Pointer_LArrow();
	Set_Pointer_Heigh_1();
}
void Move_Pointer2Power_4			(void)
{
	Move_Pointer(10,3);
	Set_Pointer_LArrow();
	Set_Pointer_Heigh_1();
}
void Move_Pointer2Power_All			(void)
{
	Move_Pointer(10,0);
	Set_Pointer_LArrow();
	Set_Pointer_Heigh_4();
}
//----------------------------------------------------------------------------------------------------
void Move_Pointer2Power_All_And_Actual_Pic_Handler_Index2Up	(void)	{Move_Pointer2Power_All();	Actual_Pic_Handler_Index2Up();}
void Move_Pointer2Powe_1_And_Actual_Pic_Handler_Index2Down	(void)	{Move_Pointer2Power_1();	Actual_Pic_Handler_Index2Down();}
void Move_Pointer2Power_4_And_Actual_Pic_Handler_Index2Up	(void)	{Move_Pointer2Power_4();	Actual_Pic_Handler_Index2Up();}

void Move_Pointer2Time_And_Actual_Pic_Handler_Index2Down	(void)	{Move_Pointer2Time();		Actual_Pic_Handler_Index2Down();}
void Move_Pointer2Player_And_Actual_Pic_Handler_Index2Down 	(void)	{Move_Pointer2Player();		Actual_Pic_Handler_Index2Down();}
void Move_Pointer2Time_And_Actual_Pic_Handler_Index2Up		(void)	{Move_Pointer2Time();		Actual_Pic_Handler_Index2Up();}
 
void V7Scroll2Up_And_Dont_Blink_Pointer				(void)	{V7Scroll2Up();			Dont_Blink_Pointer();}
void V7Scroll2Down_And_Blink_Pointer				(void)	{V7Scroll2Down();		Blink_Pointer();}
void Pasive_Power_And_Active_Time_Param				(void)	{Pasive_Power(); 		Active_Time_Param();}

void Play_Stop(void)
{
	if(!Estim_Phisical_State()) {
		Move_Pointer2Power_All();
		Set_Actual_Pic_Handler_Index(7);
		Set_Actual_Pic_Item_Index(8);
		Blink_Pointer();
		Fan_On();
	}
	else
		Fan_Off();
	Toogle_Player_State();
}
void Del_All_Layers_But_Power					(void)
{
	Del_Memory();
	Del_Programs();
	Del_Setting_Bank();
	Del_Setting_Menu();
}
//--------------Events----------------------------------

void Test(void)	{Del_Power();Add_Welcome();}
struct Struct_Pic_Events Power_Events[] RODATA=
{		
{ Invalid_Button_Code,		{Power_Constr			,Test				,Power_Destr}},
{ Rotary_CW_Code,		{String_List_Handler0		,Rien				,Rien}},
{ Rotary_ACW_Code,		{String_List_Handler1		,Rien				,Rien}},
{ Button_Rotary_Code,		{Rien				,Button_Rotary_Hold_Func	,String_List_Handler2}},
};
//--------------Pics Info----------------------------------
struct Struct_Pic Power_Pic RODATA =
{
	0,0,SLIDE_LENGTH+1,4,20,0,10,Power_Events,{Power_Data_Const}
}; 
//------------------------------------------------------
void Add_Power	(void)	
{
 	Add_Player();
	Add_Time();	
	Add_Pic_On_Top 	(&Power_Pic);
 	Send_NVDebug_Power_Pic_Data2Serial(13,"Power Add  \r\n");
}
void Del_Power	(void)	
{
	Del_Pic 	(&Power_Pic);
}
//------------------------------------------------------
