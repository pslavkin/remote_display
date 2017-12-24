#include "state_machine.h"
#include "events.h"
#include "display_phisical.h"
#include "display_pics.h"
#include "display_layers.h"
#include "schedule.h"
#include "serial_tx.h"
#include "serial_session.h"
#include "string.h"
#include "type_conversion.h"
#include "buttons.h"
#include "everythings.h"
#include "font_5x4_pic.h"

unsigned char Customs_Chars_Font_5x4[][8] RODATA=	{

	{0x00,0x00,0x00,0x00,0x1F,0x1F,0x1F,0x1F},
	{0x01,0x03,0x07,0x0F,0x1F,0x1F,0x1F,0x1F},
	{0x10,0x18,0x1C,0x1E,0x1F,0x1F,0x1F,0x1F},
	{0x1F,0x1F,0x1F,0x1F,0x00,0x00,0x00,0x00},
	{0x1F,0x1F,0x1F,0x1F,0x0F,0x07,0x03,0x01},
	{0x1F,0x1F,0x1F,0x1F,0x1E,0x1C,0x18,0x10},
	{0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F},
	};

void Init_Font_5x4		(void) 	
{
	unsigned char i;
	for (i=0;i<sizeof(Customs_Chars_Font_5x4)/8;i++) 
		Write_Custom_Char(i,(uint8_t*)Customs_Chars_Font_5x4[i]);
}
//------------------------------------------------------
char* Font_5x4_Data[] RODATA=
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
"", ///


"\x09\x0B\x0B\x0B\x0A"
"\x0E\x20\x08\x0B\x0E"
"\x0E\x0B\x20\x20\x0E"
"\x0C\x08\x08\x08\x0D",   //0

"\x20\x09\x0E\x20\x20"
"\x0B\x20\x0E\x20\x20"
"\x20\x20\x0E\x20\x20"
"\x20\x08\x0E\x08\x20",   //1

"\x0D\x0B\x0B\x0B\x0A"
"\x20\x20\x08\x0B\x20"
"\x08\x0B\x20\x20\x20"
"\x0E\x08\x08\x08\x09",   //2

"\x0D\x0B\x0B\x0B\x0E"
"\x20\x20\x08\x08\x0E"
"\x20\x20\x0B\x0B\x0E"
"\x0A\x08\x08\x08\x0E",   //3

"\x0E\x20\x20\x20\x0E"
"\x0E\x08\x08\x08\x0E"
"\x0B\x0B\x0B\x0B\x0E"
"\x20\x20\x20\x20\x0E",   //4

"\x0E\x0B\x0B\x0B\x0C"
"\x0E\x08\x08\x08\x08"
"\x0B\x0B\x0B\x0B\x0E"
"\x0A\x08\x08\x08\x0E",   //5

"\x09\x0D\x0B\x0B\x0C"
"\x0E\x08\x08\x08\x08"
"\x0E\x0B\x0B\x0C\x0E"
"\x0E\x08\x08\x08\x0E",   //6

"\x0D\x0B\x0B\x0B\x0E"
"\x20\x20\x20\x0E\x20"
"\x20\x20\x0E\x20\x20"
"\x20\x0E\x20\x20\x20",   //7

"\x09\x0B\x0B\x0B\x0A"
"\x0E\x08\x08\x08\x0E"
"\x0E\x0B\x0B\x0B\x0E"
"\x0C\x08\x08\x08\x0D",   //8

"\x0E\x0B\x0B\x0B\x0E"
"\x0E\x08\x08\x08\x0E"
"\x0B\x0B\x0B\x0B\x0E"
"\x0A\x08\x08\x08\x0E",   //9

"", // empty
"", // empty
"", // empty
"", // empty
"", // empty
"", // empty
"", // empty
"", // empty
"", // empty
"", // empty
"", // empty
"", // empty
"", // empty
"", // empty
"", // empty
"", // empty
"", // empty
"", // empty
"", // empty
"", // empty

"\x0E\x08\x20\x20\x0E"
"\x0E\x0E\x08\x20\x0E"
"\x0E\x20\x0B\x0E\x0E"
"\x0E\x20\x20\x0B\x0E",   //N

"\x09\x0D\x0B\x0C\x0A"
"\x0E\x20\x20\x20\x0E"
"\x0E\x20\x20\x20\x0E"
"\x0C\x0A\x08\x09\x0D",   //O

"", // empty
"", // empty
"", // empty
"", // empty

"\x0D\x0B\x0E\x0B\x0C"
"\x20\x20\x0E\x20\x20"
"\x20\x20\x0E\x20\x20"
"\x20\x08\x0E\x08\x20",   //T

};
//struct Struct_Pic Font_5x4	={0,0,5 ,4,0,0,1,Rien_Events,Font_5x4_Data};
//----------------------------------
