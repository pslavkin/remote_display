#include "display_phisical.h"
#include "display_layers.h"
#include "display_pics.h"
#include "string.h"
#include "buttons.h"
#include "rotary.h"
#include "setting_param_pic.h"
#include "setting_menu_pic.h"
#include "setting_bank_pic.h"
#include "blank_pic.h"
#include "tpm.h"
#include "programs_pic.h"
#include "type_conversion.h"
//----------------------------------------------------------------------------------------------------
unsigned char Customs_Chars_Setting_Menu[][8] RODATA =	{
	{0x00,0x00,0x00,0x00,0x01,0x07,0x1F,0x07},
	{0x00,0x00,0x00,0x00,0x1F,0x1F,0x1F,0x1F},
	{0x1F,0x1F,0x1F,0x1F,0x00,0x00,0x00,0x00},
	{0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F}, 
	{0x03,0x07,0x0F,0x1F,0x1F,0x1F,0x1F,0x1F}, 
	{0x1F,0x0F,0x07,0x03,0x1F,0x1F,0x1F,0x1F}, 
	};

void Init_Custom_Chars_Setting_Menu		(void) 	
{
	Write_Custom_Bank(sizeof(Customs_Chars_Setting_Menu),Customs_Chars_Setting_Menu[0]);
}
//----------------------------------------------------------------------------------------------------
struct Struct_Params 	Params;
unsigned char          	Bank_Changing;	//bool, dice si se trata de la serie 0 o 1 para modificar
unsigned char          	Bank_In_Use;	//bool, dice si se trata de la serie 0 o 1 para ejecucion
//----------------------------------------------------------------------------------------------------
void Toogle_Bank			(void)			{Bank_In_Use^=0x01;}
void Set_Bank_In_Use			(unsigned char Bank)	{Bank_In_Use=Bank;}
void Init_Setting_Menu			(void)			{Set_Bank_In_Use(0);}
struct Struct_Params* Read_Menu_Params	(void)			{return &Params;}
//----------A--RISE----------------------------------------------------------------------------------------
void Setting_Rise(void)
{
 struct Struct_Setting_Char_Param Param={1,100,1,(unsigned char*)0,Update_Setting_Char_Param,Active_Setting_Menu};
 Param.Value=&Params.A[Bank_Changing];
 Set_Setting_Char_Param(&Param);
}
unsigned char Read_A(void)	{return Params.A[Bank_In_Use];}
//---------B---PULSE--------------------------------------------------------------------------------------
void Update_Pulse(void)
{
 Update_Setting_Charx10_Param();
 Params.B[Bank_Changing]=Test_Pulse_Freq_Conflict(Params.B[Bank_Changing]);
 Set_Ftm0_Pulse_10uSec(Read_B()); 		//Cada vez que se cambia el ancho de pulso se actualiza en tpm para que tome los valores nuevos en el momento.. teniendo en cuenta que se puede estar cambiando un banco y ejecutando el otro asi que puede que no haga nada...pero tampoco pasa nada si se sobreescriben los datos que ya tenian... se podria preguntar antes.. pero prefiero asi...
 Set_Ftm1_Pulse_10uSec(Read_B()); 		//Cada vez que se cambia el ancho de pulso se actualiza en tpm para que tome los valores nuevos en el momento.. teniendo en cuenta que se puede estar cambiando un banco y ejecutando el otro asi que puede que no haga nada...pero tampoco pasa nada si se sobreescriben los datos que ya tenian... se podria preguntar antes.. pero prefiero asi...
}
void Setting_Pulse(void)
{
 struct Struct_Setting_Char_Param Param={1,40,1,(unsigned char*)0,Update_Pulse,Active_Setting_Menu};
 Param.Value=&Params.B[Bank_Changing];
 Set_Setting_Char_Param(&Param);
}
unsigned char Read_B(void)	{return Params.B[Bank_In_Use];}
//----------C--FREC--------------------------------------------------------------------------------------
void Setting_Frequency_Exit(void)
{
 Del_Setting_Int_Param();
 Unhide_Setting_Char_Param();
}
void Update_Frequency(void)
{
 Params.C[Bank_Changing]=Test_Freq_Pulse_Conflict(Params.C[Bank_Changing]);
 Set_Ftm0_Period_Hz(Read_C()); 						//Cada vez que se cambia la frec de un banco, por las dudas se actualiza la frec en ejecucion por si coinciden la de cambi con la de ejecucion...se podria preguntar antes tambien...pero si se cambia igual no genera molestia porque se estaria poniendo el mismo valor que tenia y eso no genera flickeo ni nada...
 Set_Ftm1_Period_Hz(Read_C()); 						//Cada vez que se cambia la frec de un banco, por las dudas se actualiza la frec en ejecucion por si coinciden la de cambi con la de ejecucion...se podria preguntar antes tambien...pero si se cambia igual no genera molestia porque se estaria poniendo el mismo valor que tenia y eso no genera flickeo ni nada...
 Update_Setting_Int_Param();
}

void Setting_Frequency(void)
{
 struct Struct_Setting_Int_Param Param={2,3000,1,(unsigned int*)0,Update_Frequency,Active_Setting_Menu};
 Param.Value=&Params.C[Bank_Changing];
 Hide_Setting_Char_Param();
 Set_Setting_Int_Param(&Param);
 Add_Setting_Int_Param();
 Pasive_Setting_Int_Param();
}
unsigned int Read_C(void)	{return Params.C[Bank_In_Use];}
//----------D--CONTRACTION--------------------------------------------------------------------------------------
void Setting_Contraction(void)
{
 struct Struct_Setting_Char_Param Param={0,120,1,(unsigned char*)0,Update_Setting_Char_Param,Active_Setting_Menu};
 Param.Value=&Params.D[Bank_Changing];
 Set_Setting_Char_Param(&Param);
}
unsigned char Read_D(void)	{return Params.D[Bank_In_Use];}
//----------E--FALL--------------------------------------------------------------------------------------
void Setting_Fall(void)
{
 struct Struct_Setting_Char_Param Param={1,100,1,(unsigned char*)0,Update_Setting_Char_Param,Active_Setting_Menu};
 Param.Value=&Params.E[Bank_Changing];
 Set_Setting_Char_Param(&Param);
}
unsigned char Read_E(void)	{return Params.E[Bank_In_Use];}
//----------F--SLEEP--------------------------------------------------------------------------------------
void Setting_Sleep(void)
{
 struct Struct_Setting_Char_Param Param={0,99,1,(unsigned char*)0,Update_Setting_Char_Param,Active_Setting_Menu};
 Param.Value=&Params.F[Bank_Changing];
 Set_Setting_Char_Param(&Param);
}
unsigned char Read_F(void)	{return Params.F[Bank_In_Use];}
//----------G--REPETITION--------------------------------------------------------------------------------------
void Setting_Repetition(void)
{
 struct Struct_Setting_Char_Param Param={0,99,1,(unsigned char*)0,Update_Setting_Char_Param,Active_Setting_Menu};
 Param.Value=&Params.G[Bank_Changing];
 Set_Setting_Char_Param(&Param);
}
unsigned char Read_G(void)	{return Params.G[Bank_In_Use];}
//----------H--SEQUENCE--------------------------------------------------------------------------------------
void Update_Sequence(void)
{
 Update_Setting_Char_Param();
 Reset_Sequence_Index();						//tengo que reiniciar el indice que recorre el efecto porque los efectos son de distinta longitud y podria salirse fuera de la cadena... 
}
void Setting_Sequence(void)
{
 struct Struct_Setting_Char_Param Param={1,MAX_CHANNEL_SEQUENCE,1,(unsigned char*)0,Update_Sequence,Active_Setting_Menu};
 Param.Value=&Params.H[Bank_Changing];
 Set_Setting_Char_Param(&Param);
}
unsigned char Read_H(void)	{return Params.H[Bank_In_Use]-1;} //menos uno porque tomo origien 1 para mejorar experiencia usuario!?
//----------I--SEQUENCE REPETITION--------------------------------------------------------------------------------------
void Setting_Sequence_Times(void)
{
 struct Struct_Setting_Char_Param Param={0,200,1,(unsigned char*)0,Update_Setting_Char_Param,Active_Setting_Menu};
 Param.Value=&Params.I[Bank_Changing];
 Set_Setting_Char_Param(&Param);
}
unsigned char Read_I(void)	{return Params.I[Bank_In_Use];}
//----------J--PHASE--------------------------------------------------------------------------------------
void Update_Phase(void)
{
 Update_Setting_Char_Param();
 Set_Phase_Ftm1(Read_J());
}
void Setting_Phase(void)
{
 struct Struct_Setting_Char_Param Param={0,180,1,(unsigned char*)0,Update_Phase,Active_Setting_Menu};
 Param.Value=&Params.J[Bank_Changing];
 Set_Setting_Char_Param(&Param);
}
unsigned char Read_J(void)	{return Params.J[Bank_In_Use];} //menos uno porque tomo origien 1 para mejorar experiencia usuario!?
//--------------Pics Data----------------------------------
struct Struct_String_List Setting_Menu_Icon_Data_Const[] RODATA=
{
//	1,"1-Rampa ascendente [100mseg]   "	    	,Setting_Rise		,Rien			,Rien}},
	{0,(unsigned char*)"\x20\x20\x20\x20\x20\x08\x08\x20"	,{Rien			,Rien			,Rien}},
	{0,(unsigned char*)"\x20\x20\x20\x08\x0A\x0A\x0A\x20"	,{Rien			,Rien			,Rien}},
	{0,(unsigned char*)"\x20\x08\x0A\x0A\x0A\x0A\x0A\x20"	,{Rien			,Rien			,Rien}},
//	{1,"2-Pulso [useg]   "		    		,Setting_{Pulse		,Rien			,Rien}},
	{0,(unsigned char*)"\x20\x08\x08\x08\x08\x20\x20\x20"	,{Rien			,Rien			,Rien}},
	{0,(unsigned char*)"\x08\x0A\x20\x20\x0A\x20\x20\x08"	,{Rien			,Rien			,Rien}},
	{0,(unsigned char*)"\x20\x20\x20\x20\x0A\x08\x08\x0A"	,{Rien			,Rien			,Rien}},
//	{1,"3-Frecuencia [Hz]   "	            	,Setting_{Pulse		,Rien			,Rien}},
	{0,(unsigned char*)"\x20\x20\x20\x20\x20\x20\x20\x20"	,{Rien			,Rien			,Rien}},
	{0,(unsigned char*)"\x08\x09\x08\x20\x08\x20\x20\x20"	,{Rien			,Rien			,Rien}},
	{0,(unsigned char*)"\x20\x20\x20\x09\x20\x20\x20\x20"	,{Rien			,Rien			,Rien}},
//	{1,"4-Contraccion [seg]   "	            	,Setting_{Contraction	,Rien			,Rien}},
	{0,(unsigned char*)"\x20\x08\x08\x08\x08\x08\x08\x20"	,{Rien			,Rien			,Rien}},
	{0,(unsigned char*)"\x0A\x0A\x0A\x0A\x0A\x0A\x0A\x0A"	,{Rien			,Rien			,Rien}},
	{0,(unsigned char*)"\x0A\x0A\x0A\x0A\x0A\x0A\x0A\x0A"	,{Rien			,Rien			,Rien}},
//	{1,"5-Rampa descendente [100mseg]    "		,Setting_{Fall		,Rien			,Rien}},
	{0,(unsigned char*)"\x20\x08\x08\x20\x20\x20\x20\x20"	,{Rien			,Rien			,Rien}},
	{0,(unsigned char*)"\x20\x0A\x0A\x0A\x08\x20\x20\x20"	,{Rien			,Rien			,Rien}},
	{0,(unsigned char*)"\x20\x0A\x0A\x0A\x0A\x0A\x08\x20"	,{Rien			,Rien			,Rien}},
//	{1,"6-Descanso [seg]   "		    		,{Setting_Sleep		,Rien			,Rien}},
	{0,(unsigned char*)"\x20\x20\x20\x20\x20\x20\x20\x20"	,{Rien			,Rien			,Rien}},
	{0,(unsigned char*)"\x20\x20\x20\x20\x20\x20\x20\x20"	,{Rien			,Rien			,Rien}},
	{0,(unsigned char*)"\x0A\x08\x08\x08\x08\x08\x08\x0A"	,{Rien			,Rien			,Rien}},
//	{0,"7-Secuencia	       "	            	,Setting_{Sequence	,Rien			,Enable_Setting_Char_Change}},
	{0,(unsigned char*)"\x20\x20\x08\x20\x20\x20\x20\x20"	,{Rien			,Rien			,Rien}},
	{0,(unsigned char*)"\x20\x20\x0A\x20\x08\x20\x0A\x20"	,{Rien			,Rien			,Rien}},
	{0,(unsigned char*)"\x0A\x20\x0A\x20\x0A\x20\x0A\x20"	,{Rien			,Rien			,Rien}},
//	{0,"8-Repetir Secuencia "	            	,Setting_{Sequence_Times	,Rien			,Enable_Setting_Char_Change}},
	{0,(unsigned char*)"\x08\x08\x08\x08\x08\x08\x08\x08"	,{Rien			,Rien			,Rien}},
	{0,(unsigned char*)"\x0A\x20\x20\x08\x0B\x08\x08\x09"	,{Rien			,Rien			,Rien}},
	{0,(unsigned char*)"\x0A\x08\x08\x08\x0C\x08\x08\x08"	,{Rien			,Rien			,Rien}},
//	{0,"9-Fase [grados]"		            	,Setting_{Phase		,Rien			,Enable_Setting_Char_Change}},
	{0,(unsigned char*)"\x20\x20\x20\x20\x20\x20\x20\x20"	,{Rien			,Rien			,Rien}},
	{0,(unsigned char*)"\x08\x08\x08\x0A\x09\x09\x09\x09"	,{Rien			,Rien			,Rien}},
	{0,(unsigned char*)"\x0A\x09\x09\x09\x09\x0A\x08\x08"	,{Rien			,Rien			,Rien}},
//	{1,"10-Repeticiones [veces]   "	            	,Setting_{Repetition	,Rien			,Rien}},
	{0,(unsigned char*)"\x08\x08\x08\x08\x08\x08\x08\x08"	,{Rien			,Rien			,Rien}},
	{0,(unsigned char*)"\x0A\x20\x20\x08\x0B\x08\x08\x09"	,{Rien			,Rien			,Rien}},
	{0,(unsigned char*)"\x0A\x08\x08\x08\x0C\x08\x08\x08"	,{Rien			,Rien			,Rien}},
	{0,(unsigned char*)""					,{Rien			,Rien			,Rien}},
};
struct Struct_String_List Setting_Menu_Label_Data_Const[] RODATA=
{
	{0,(unsigned char*)"1-Rampa asc.  [0.1s]"	    		,{Setting_Rise			,Rien			,Enable_Setting_Char_Change}},
	{0,(unsigned char*)"2-Pulso         [us]"	    		,{Setting_Pulse			,Rien			,Enable_Setting_Char_Change}},
	{0,(unsigned char*)"3-Frecuencia    [Hz]"		    	,{Setting_Frequency		,Setting_Frequency_Exit	,Enable_Setting_Int_Change}},
	{0,(unsigned char*)"4-Contraccion    [s]"		    	,{Setting_Contraction		,Rien			,Enable_Setting_Char_Change}},
	{0,(unsigned char*)"5-Rampa desc. [0.1s]"	 		,{Setting_Fall			,Rien			,Enable_Setting_Char_Change}},
	{0,(unsigned char*)"6-Descanso     [seg]"	    		,{Setting_Sleep			,Rien			,Enable_Setting_Char_Change}},
	{0,(unsigned char*)"7-Secuencia         "		    	,{Setting_Sequence		,Rien			,Enable_Setting_Char_Change}},
	{0,(unsigned char*)"8-Repetir Secuencia "		    	,{Setting_Sequence_Times	,Rien			,Enable_Setting_Char_Change}},
	{0,(unsigned char*)"9-Fase      [grados]"		    	,{Setting_Phase			,Rien			,Enable_Setting_Char_Change}},
	{0,(unsigned char*)"10-Repeticiones     "		    	,{Setting_Repetition		,Rien			,Enable_Setting_Char_Change}},
	{0,(unsigned char*)""						,{Rien				,Rien			,Enable_Setting_Char_Change}},
};
//---------------Functions---------------------------------------
void Setting_Menu_Constr(void)	
{
	Init_Custom_Chars_Setting_Menu();
	Add_Setting_Char_Param();
	Pasive_Setting_Char_Param();
	Setting_Menu_Label_Data_Const[0].Handler[0]();
}
void Setting_Menu_Destr(void)	
{
	Del_Setting_Char_Param();
	Del_Blank();
}
void Set_Bank_Changing(unsigned char Bank)
{
	Bank_Changing=Bank;
}
void Active_Setting_Menu	(void)	{Active_Pic (&Setting_Menu_Label_Pic);}
void Pasive_Setting_Menu	(void)	{Pasive_Pic (&Setting_Menu_Label_Pic);}

void Update_Bank_Parameters(void)
{
	Set_Ftm0_Period_Hz(Read_C());		//si cambio de banco tengo que actualizar los parametros que no se pulean... que son puslo y frec....
	Set_Ftm1_Period_Hz(Read_C());
	Set_Ftm0_Pulse_10uSec(Read_B());
	Set_Ftm1_Pulse_10uSec(Read_B());
	Set_Phase_Ftm1(Read_J());
}

void Set_Bank(struct Struct_Params* Program)
{
	Params=*Program;
	Bank_In_Use=0;
	Update_Bank_Parameters();
}
void Enable_Setting_Char_Change(void)
{
	Pasive_Setting_Menu();
	Active_Setting_Char_Param();
}
void Enable_Setting_Int_Change(void)
{
	Pasive_Setting_Menu();
	Active_Setting_Int_Param();
}
//------------AND----------------------------------------------------------------------------------------
void Scroll2Down_Icon_Label_And_String_List_Handler0	(void)	
{
	String_List_Handler1();
	V3Scroll2Down_Pic(&Setting_Menu_Icon_Pic);
	V1Scroll2Down();
	String_List_Handler0();
}
void Scroll2Up_Icon_Label_And_String_List_Handler0	(void)	
{
	String_List_Handler1();
	V3Scroll2Up_Pic(&Setting_Menu_Icon_Pic);
	V1Scroll2Up();
	String_List_Handler0();
}
//--------------Events----------------------------------
struct Struct_Pic_Events Setting_Menu_Label_Events[] RODATA=
{		
{ Invalid_Button_Code,		{Setting_Menu_Constr					,Rien			,Setting_Menu_Destr}},
{ Rotary_CW_Code,		{Scroll2Down_Icon_Label_And_String_List_Handler0	,Rien			,Rien}},
{ Rotary_ACW_Code,		{Scroll2Up_Icon_Label_And_String_List_Handler0		,Rien			,Rien}},
{ Button_Rotary_Code,		{Rien							,Del_Setting_Menu	,String_List_Handler2}},
};
//--------------Pics Info----------------------------------

struct Struct_Pic Setting_Menu_Icon_Pic	RODATA	={0,1,8 ,4,0,0,1,Rien_Events			,{Setting_Menu_Icon_Data_Const }};
struct Struct_Pic Setting_Menu_Label_Pic	={0,0,20,1,0,0,4,Setting_Menu_Label_Events	,{Setting_Menu_Label_Data_Const}};
//------------------------------------------------------
void Add_Setting_Menu	(void)	
{
	Add_Blank();
	Add_Pic_On_Top 	(&Setting_Menu_Icon_Pic);
	Add_Pic_On_Top	(&Setting_Menu_Label_Pic);
}
void Del_Setting_Menu	(void)	
{
	Pic_String_List_Handler1(&Setting_Menu_Label_Pic);
	Del_Pic 	(&Setting_Menu_Icon_Pic);
	Del_Pic 	(&Setting_Menu_Label_Pic);
}
//------------------------------------------------------
