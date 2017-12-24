#include "state_machine.h"
#include "events.h"
#include "display_pics.h"
#include "display_phisical.h"
#include "display_layers.h"
#include "schedule.h"
#include "serial_tx.h"
#include "serial_session.h"
#include "string.h"
#include "type_conversion.h"
#include "buttons.h"
#include "everythings.h"
#include "setting_param_pic.h"

unsigned char Customs_Chars_Font_3x4[][8] RODATA=	{

	{0x00,0x00,0x00,0x00,0x1F,0x1F,0x1F,0x1F}, 
	{0x1F,0x1F,0x1F,0x1F,0x00,0x00,0x00,0x00}, 
	{0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F}, 
};

void Init_Font_3x4		(void) 	
{
	unsigned char i;
	for (i=0;i<sizeof(Customs_Chars_Font_3x4)/8;i++) 
		Write_Custom_Char(i,(uint8_t*)Customs_Chars_Font_3x4[i]);
}
//------------------------------------------------------
char* Font_3x4_Data[] RODATA=
{
"", // 
"", //!
"", //"
"", //#
"", //$
"", //%
"", //&
"", //'
"", //(
"", //)
"", //*
"", //+
"", //,
"", //-
"", //.

"\x20\x20\x20\x20"
"\x20\x20\x20\x20"
"\x20\x20\x20\x20",   ///


"\x0A\x09\x0A\x20"
"\x0A\x20\x0A\x20"
"\x0A\x08\x0A\x20",   //0

"\x20\x08\x0A\x20"
"\x20\x20\x0A\x20"
"\x20\x20\x0A\x20",   //1

"\x09\x09\x0A\x20"
"\x0A\x0A\x0A\x20"
"\x0A\x08\x08\x20",   //2

"\x09\x09\x0A\x20"
"\x0A\x0A\x0A\x20"
"\x08\x08\x0A\x20",   //3

"\x0A\x20\x0A\x20"
"\x0A\x0A\x0A\x20"
"\x20\x20\x0A\x20",   //4

"\x0A\x09\x09\x20"
"\x0A\x0A\x0A\x20"
"\x08\x08\x0A\x20",   //5

"\x0A\x09\x09\x20"
"\x0A\x0A\x0A\x20"
"\x0A\x08\x0A\x20",   //6

"\x0A\x09\x0A\x20"
"\x20\x20\x0A\x20"
"\x20\x20\x0A\x20",   //7

"\x0A\x09\x0A\x20"
"\x0A\x0A\x0A\x20"
"\x0A\x08\x0A\x20",   //8

"\x0A\x09\x0A\x20"
"\x0A\x0A\x0A\x20"
"\x20\x20\x0A\x20",   //9

};
struct Struct_Pic Font_3x4	={0,0,4,3,0,0,1,Rien_Events,{.Font=Font_3x4_Data}};
//----------------------------------
