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

unsigned char Customs_Chars_Font_3x3[][8] RODATA=	{

	{0x00,0x00,0x00,0x00,0x1F,0x1F,0x1F,0x1F}, 
	{0x1F,0x1F,0x1F,0x1F,0x00,0x00,0x00,0x00}, 
	{0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F}, 
};

void Init_Font_3x3		(void) 	
{
	unsigned char i;
	for (i=0;i<sizeof(Customs_Chars_Font_3x3)/8;i++) 
		Write_Custom_Char(i,(uint8_t*)Customs_Chars_Font_3x3[i]);
}
//------------------------------------------------------
char* Font_3x3_Data[] RODATA =
{
"", // 
"\x01", //slide //!
"", //arrow //"
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
"", ///


"\x0A\x09\x0A"
"\x0A\x20\x0A"
"\x0A\x08\x0A",   //0

"\x20\x08\x0A"
"\x20\x20\x0A"
"\x20\x20\x0A",   //1

"\x09\x09\x0A"
"\x0A\x0A\x0A"
"\x0A\x08\x08",   //2

"\x09\x09\x0A"
"\x0A\x0A\x0A"
"\x08\x08\x0A",   //3

"\x0A\x20\x0A"
"\x0A\x0A\x0A"
"\x20\x20\x0A",   //4

"\x0A\x09\x09"
"\x0A\x0A\x0A"
"\x08\x08\x0A",   //5

"\x0A\x09\x09"
"\x0A\x0A\x0A"
"\x0A\x08\x0A",   //6

"\x0A\x09\x0A"
"\x20\x20\x0A"
"\x20\x20\x0A",   //7

"\x0A\x09\x0A"
"\x0A\x0A\x0A"
"\x0A\x08\x0A",   //8

"\x0A\x09\x0A"
"\x0A\x0A\x0A"
"\x20\x20\x0A",   //9

};
//struct Struct_Pic Font_3x3	={0,0,3,3,0,0,1,Rien_Events,Font_3x3_Data};
//----------------------------------
