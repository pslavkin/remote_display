#include "display_pics.h"
#include "type_conversion.h"
#include "events.h"
#include "tpanel.h"
#include "display_layers.h"
#include "stdint.h"
#include "pass_pic.h"
#include "debug.h"
#include <string.h>
#include "str.h"
#include "log_pic.h"
#include "font_30x42.h"
#include "serial_tx.h"
//------------------------------------------------------
uint16_t *Pass_Data[9]; //con 8 alcanza
char Pass_String[9]      = "";

void Pass_Destr(void)
{
}
void Pass_Constr(void)
{
   String_Copy_Until(Read_RLog()->Line[0],Read_Pass_String(),' ',9);
   Pass_String2Pic();
}
struct Struct_Pic_Events Pass_Events[] =
{
 {{0 ,0   ,0 ,0}  ,Invalid_Button ,0 ,{Pass_Constr ,Rien ,Pass_Destr}} ,// On_Create
 {{0 ,239 ,0 ,42} ,Invalid_Button ,1 ,{Rien        ,Rien ,Add_Log}}    ,
};

char* Read_Pass_String(void)
{
   return Pass_String;
}
struct Struct_Pic Pass_Pic  =
{
   {0,0+29,5,5+41},0,0,2,Pass_Events,8,Pass_Data
};

void Pass_String2Pic(void)
{
   char Buff[sizeof ( Pass_String )];
   String_Padd ( Pass_String ,Buff      ,sizeof(Pass_String ),' ');
   String2Pic  ( Buff        ,&Pass_Pic ,&Font_30x42_Pic );
}

void Add_Pass ( void ) { Add_Pic_On_Top(&Pass_Pic);}
void Del_Pass ( void ) { Del_Pic(&Pass_Pic)       ;}

void Add_Pass_Digit(uint8_t Digit)
{
   Append_Data     ( Pass_String,Digit,sizeof(Pass_String) );
   Pass_String2Pic (                   );
}
void Del_Pass_Digit(void)
{
   Backspace_Data(Pass_String);
   Pass_String2Pic();
}
void Add_Pass_Digit0(void) {Add_Pass_Digit('0');}
void Add_Pass_Digit1(void) {Add_Pass_Digit('1');}
void Add_Pass_Digit2(void) {Add_Pass_Digit('2');}
void Add_Pass_Digit3(void) {Add_Pass_Digit('3');}
void Add_Pass_Digit4(void) {Add_Pass_Digit('4');}
void Add_Pass_Digit5(void) {Add_Pass_Digit('5');}
void Add_Pass_Digit6(void) {Add_Pass_Digit('6');}
void Add_Pass_Digit7(void) {Add_Pass_Digit('7');}
void Add_Pass_Digit8(void) {Add_Pass_Digit('8');}
void Add_Pass_Digit9(void) {Add_Pass_Digit('9');}

void Print_Actual_Pass(void)
{
   Send_NVData2Serial(strlen(Read_Pass_String()),Read_Pass_String());
   Send_NLine2Serial();
}
