#include "display_phisical.h"
#include "display_layers.h"
#include "display_pics.h"
#include "buttons.h"
#include "rotary.h"
#include "setting_menu_pic.h"
#include "programs_pic.h"
#include "power_pic.h"
#include "serial_tx.h"
#include "blank_pic.h"
#include "time_pic.h"
#include "programs_pic.h"
#include "setting_bank_pic.h"
#include "setting_param_pic.h"
#include "font_slide_1x1_pic.h"
#include "type_conversion.h"

static unsigned char Actual_Program;
static unsigned char Sequence_Index;
//----------------------------------------------------------------------------------------------------
char* Channel_Sequence[MAX_CHANNEL_SEQUENCE] RODATA =	//define efectos para las diferentes secuencias que se pueden elegir. es decir como se ejecutan los canales en cada pasada..termina con 0xFF.
{
	"\x0F\xFF",								//todos
	"\x01\x02\x04\x08\xFF",						//secuencia simple
	"\x03\x0C\xFF",							//2 y 2
	"\x05\x0A\xFF",							//2 y 2
	"\x01\x02\x04\x08\x04\x02\xFF",					//auto fantastico
	"\x01\x02\x04\x08\x0F\x0F\xFF",					//secuencia y todos al final
	"\x01\x03\x07\x0F\x0E\x0C\x08\x0C\x0E\x0F\x07\x03\xFF"		//auto fantastico acumulativo
};	
void	Reset_Sequence_Index(void)
{
	Sequence_Index=0;
}
unsigned char Actual_Sequence_Data(void)
{
	return Channel_Sequence[Read_H()][Sequence_Index];
}
unsigned char Next_Sequence_Channel(void)
{
	Sequence_Index++;
	if(Actual_Sequence_Data()==0xFF) Sequence_Index=0;
	return Sequence_Index==0;
}
//----------------------------------------------------------------------------------------------------
struct Struct_Params Programs[] RODATA =
{
//	 rampa  pulso   freq1   contr   rampa    desca   repe  efec  efecT phase
 	{{1 ,1 },{40,1 },{5  , 2},{120, 1},{ 1 , 1},{ 0,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{50 , 2},{  6, 6},{ 10, 1},{ 2,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,10},{20,1 },{25 , 4},{  5, 8},{ 10, 1},{ 1,1},{1,1},{1,1},{1,1},{180,180}}, //
 	{{10,10},{20,20},{90 , 2},{  4, 1},{ 10,10},{ 2,2},{1,1},{2,2},{1,1},{180,180}}, //
 	{{ 1,1 },{20,20},{ 2 ,60},{ 10, 4},{  1, 1},{ 1,1},{1,1},{1,1},{1,1},{180,180}}, //
 	{{10,10},{20,20},{ 3 ,55},{ 14, 5},{ 10,10},{ 2,2},{1,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{80 , 2},{  4, 1},{ 10, 1},{ 2,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{40 , 2},{  4, 1},{ 10, 1},{ 2,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{75 , 2},{  3, 1},{ 10, 1},{ 2,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,10},{20,20},{ 2 ,20},{ 15, 4},{ 10,10},{ 2,2},{1,1},{1,1},{1,1},{180,180}}, //
 	{{10,10},{20,20},{ 6 ,25},{ 15, 5},{ 10,10},{ 1,1},{1,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{ 6 , 2},{  0, 1},{ 10, 1},{ 1,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{60 , 2},{  4, 1},{ 10, 1},{ 2,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{90 , 2},{  4, 1},{ 10, 1},{15,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{110, 2},{  3, 1},{ 10, 1},{30,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{50 , 2},{  2, 1},{ 10, 1},{ 2,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{85 , 2},{  4, 1},{ 10, 1},{ 4,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{70 , 2},{  3, 1},{ 10, 1},{ 2,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{60 , 2},{  3, 1},{ 10, 1},{ 5,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{55 , 2},{  5, 1},{ 10, 1},{ 2,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{40 , 2},{  2, 1},{ 10, 1},{ 1,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,10},{20,20},{ 3 ,40},{ 10, 5},{ 10,10},{ 5,5},{1,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{30 , 2},{  3, 1},{ 10, 1},{ 2,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,10},{20,20},{ 7 , 4},{ 20,20},{ 10,10},{ 1,1},{1,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{40 , 2},{  3, 1},{ 10, 1},{ 2,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{ 3 , 2},{ 20, 1},{ 10, 1},{ 1,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{80 , 2},{  0, 1},{ 10, 1},{ 1,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{95 , 2},{ 20, 1},{ 10, 1},{ 1,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{75 , 2},{  0, 1},{ 10, 1},{ 1,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{65 , 2},{  0, 1},{ 10, 1},{ 1,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{80 , 2},{  0, 1},{ 10, 1},{ 1,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{95 , 2},{ 20, 1},{ 10, 1},{ 2,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{ 2 , 2},{  0, 1},{ 10, 1},{ 1,1},{0,1},{1,1},{1,1},{180,180}}, //
 	{{10,1 },{20,1 },{75 , 2},{  0, 1},{ 10, 1},{ 1,1},{0,1},{1,1},{1,1},{180,180}}, //
};

//#pragma section {Eeprom}
//struct Struct_Params Eeprom_Programs[]= {
////	 rampa  pulso freq[int]   contr   rampa    desca   repe  efec  efecT phase
// 	{48,49,	50,51,	 3,30,  1,6,    1,6,	 1,5,	0,2,   1,1,  4,2,  180,0},
// 	{10,20,	10,20,	30,30,	5,6,	5,6,	10,5,	4,6,   1,1,  4,2,  50,0},
// 	{10,20,	10,20,	30,30,	5,6,	5,6,	10,5,	4,6,   1,1,  4,2,  30,0},
// 	{10,20,	10,20,	30,30,	5,6,	5,6,	10,5,	4,6,   1,1,  4,2,  10,0},
// 	{10,20,	10,20,	30,30,	5,6,	5,6,	10,5,	4,6,   1,1,  4,2,  8,16},
//	};
//#pragma section {}

void Print_Eeprom_Programs(void)
{
// Send_NVData2Serial(sizeof(struct Struct_Params),(unsigned char*)Eeprom_Programs);
}
//---------------------------------------------------------------------------------------------------
void Setting_Program_Num(void)
{
 struct Struct_Setting_Char_Param Param ={1,39,1,(unsigned char*)0,Update_Setting_Char_Param,Rien};
 Param.Value=&Actual_Program;
 Set_Setting_Char_Param(&Param);
}
//--------------Pics Data----------------------------------
struct Struct_String_List Programs_Data_Const[] RODATA=
{
{	0,(uint8_t*)"01-Celulitis        "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"02-Celulitis 2      "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"03-Fuerza Muscular  "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"04-Flacidez         "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"05-Modelacion       "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"06-Reafirmante      "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"07-Reafirmante 2    "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"08-Reductor         "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"09-Tonificacion     "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"10-Tonificacion 2   "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"11-Aerobico         "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"12-Calentamiento    "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"13-Fuerza           "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"14-Fuerza maxima    "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"15-Fuerza explosiva "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"16-GYM              "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"17-Hipertrofia      "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"18-Musculacion      "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"19-Potencia         "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"20-Resistencia      "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"21-Circulatorio     "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"22-Drenaje          "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"23-Drenaje 2        "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"24-Pesadez          "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"25-Post-Yeso        "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"26-Punto Motor      "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"27-Descontracturante"	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"28-Dolor agudo      "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"29-Dolor cervical   "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"30-Dolor ciatico    "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"31-Dolor de espalda "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"32-Dolor lumbar     "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"33-Relax            "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"34-Tens Endorfinico "	,{Rien		,Rien		,Select_Program}},
{	0,(uint8_t*)"35-Personalizado 1  "	,{Rien		,Rien		,Select_Eeprom_Program}},
{	0,(uint8_t*)"36-Personalizado 2  "	,{Rien		,Rien		,Select_Eeprom_Program}},
{	0,(uint8_t*)"37-Personalizado 3  "	,{Rien		,Rien		,Select_Eeprom_Program}},
{	0,(uint8_t*)"38-Personalizado 4  "	,{Rien		,Rien		,Select_Eeprom_Program}},
{	0,(uint8_t*)"39-Personalizado 5  "	,{Rien		,Rien		,Select_Eeprom_Program}},
{	0,(uint8_t*)""				,{Rien		,Rien		,Rien}},
};
//---------------Functions---------------------------------------
void Programs_Constr(void)
{
	Add_Setting_Char_Param();
	Pasive_Setting_Char_Param();
	Setting_Program_Num();
	Set_Pic_Item_And_Handler_Index(&Programs_Pic,Actual_Program-1);
	Set_Bank(&Programs[Actual_Program-1]);
}
void Programs_Destr	(void)
{
	Del_Setting_Char_Param();
	Del_Blank();
	Init_Custom_Chars_Programs(); 	
	Del_Setting_Bank();
}
void Select_Eeprom_Program		(void)  
{
//	Set_Bank(&Eeprom_Programs[Actual_Program-1-sizeof(Programs)/sizeof(struct Struct_Params)]);
//	Del_Programs();

}
void Save_Banks2Eeprom_Program		(unsigned char Eeprom_Program_Index)  
{
//	Save_Eeprom_String((unsigned char*)&Eeprom_Programs[Eeprom_Program_Index],(unsigned char*)Read_Menu_Params(),sizeof(struct Struct_Params));
}
void Select_Program		(void)  
{
	Set_Bank(&Programs[Actual_Program-1]);
	Del_Programs();
}
void V1Scroll2Up_And_Dec_Setting_Param		(void)	{V1Scroll2Up();		Dec_Setting_Char_Param();}
void V1Scroll2Down_And_Inc_Setting_Param	(void)	{V1Scroll2Down();	Inc_Setting_Char_Param();}
//--------------Events----------------------------------
struct Struct_Pic_Events Programs_Events[] RODATA=
{		
{ Invalid_Button_Code,		{Programs_Constr				,Rien			,Programs_Destr}},
{ Rotary_ACW_Code,		{V1Scroll2Up_And_Dec_Setting_Param	,Rien			,Rien}},
{ Rotary_CW_Code,		{V1Scroll2Down_And_Inc_Setting_Param	,Rien			,Rien}},
{ Button_Rotary_Code,		{Rien					,Del_Programs		,String_List_Handler2}},
};
//--------------Pics Info----------------------------------
struct Struct_Pic Programs_Pic RODATA={0,0,20,1,0,0,4,Programs_Events,{Programs_Data_Const}};
//------------------------------------------------------
void Init_Programs	(void)	
{
	Actual_Program=1;
	Sequence_Index=0;
	Set_Bank(&Programs[Actual_Program-1]);
}
void Add_Programs	(void)	{Add_Blank();	Add_Pic_On_Top	(&Programs_Pic);}
void Del_Programs	(void)	{		Del_Pic 	(&Programs_Pic);}
//------------------------------------------------------
