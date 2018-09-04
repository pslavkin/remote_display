#include "display_pics.h"
#include "type_conversion.h"
#include "events.h"
#include "tpanel.h"
#include "display_layers.h"
#include "stdint.h"
#include "Log_pic.h"
#include "debug.h"
#include <string.h>
#include "str.h"
#include "font_30x42.h"
//------------------------------------------------------
uint16_t *Log_Data[8];

struct Struct_Pic_Events Log_Events[] =
{
 {{0 ,0 ,0 ,0} ,Invalid_Button ,0 ,{Rien,Rien ,Rien}} ,// On_Create
};

struct Struct_Pic Log_Pic  =
{
   {0,0+29,18,18+41},0,0,1,Log_Events,8,Log_Data
};

void Log_String2Pic(void)
{
   char Buff[sizeof ( Log_String )];
   String_Padd ( Log_String ,Buff      ,sizeof(Log_String ),' ');
   String2Pic  ( Buff        ,&Log_Pic ,&Font_30x42_Pic );
}
void Init_Log(void)
{
   Log_String[0]='\0';
   Log_String2Pic();
}

void Add_Log ( void ) { Add_Pic_On_Top(&Log_Pic);}
void Del_Log ( void ) { Del_Pic(&Log_Pic)       ;}

void Add_Log_Digit(uint8_t Digit)
{
   Append_Data     ( Log_String,Digit,sizeof(Log_String) );
   Log_String2Pic (                   );
}
void Del_Log_Digit(void)
{
   Backspace_Data(Log_String);
   Log_String2Pic();
}
bool Psw_Compare(void)
{
   return strcmp(Log_String,Key_String)==0;
}




