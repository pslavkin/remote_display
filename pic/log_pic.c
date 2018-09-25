#include "display_pics.h"
#include "type_conversion.h"
#include "events.h"
#include "tpanel.h"
#include "display_layers.h"
#include "stdint.h"
#include "log_pic.h"
#include "debug.h"
#include <string.h>
#include "str.h"
#include "bkgd_pic.h"
#include "font_22x30.h"
#include "pass_pic.h"
#include "serial_tx.h"
//------------------------------------------------------
__attribute__((section(".data.FLEX_RAM")))
struct Log_Table RLog;
uint16_t* Log_Data[LOG_LINES][10];

struct Log_Table* Read_RLog(void)
{
   return &RLog;
}
void Print_RLog(void)
{
   Send_NLine_NVData2Serial(sizeof(RLog),RLog.Line[0]);
}
void Copy_Pass2Log(char* Pass, bool Accepted)
{
   uint8_t i;
   for(i=LOG_LINES-1;i>0;i--)
      strcpy(RLog.Line[i],RLog.Line[i-1]);
   strcpy(RLog.Line[0],Pass);
   String_Padd(RLog.Line[0] ,RLog.Line[0]     ,9 ,' ');
   Append_Data(RLog.Line[0] ,Accepted?'+':'-' ,10);
}
void Copy_Log2Pass(uint8_t N)
{
   String_Copy_Until(RLog.Line[N],Read_Pass_String(),' ',9);
   Pass_String2Pic();
   Del_Log();
}
void Copy_Log2Pass0(void) {Copy_Log2Pass(0);}
void Copy_Log2Pass1(void) {Copy_Log2Pass(1);}
void Copy_Log2Pass2(void) {Copy_Log2Pass(2);}
void Copy_Log2Pass3(void) {Copy_Log2Pass(3);}
void Copy_Log2Pass4(void) {Copy_Log2Pass(4);}

struct Struct_Pic_Events Log_Events[] =
{
 { {0  ,0   ,0    ,0}      ,Invalid_Button ,0 ,{Init_Log ,Rien ,Rien}           } ,// On_Create
 { { 0 ,239 ,0*63 ,1*63+1} ,Invalid_Button ,1 ,{Rien     ,Rien ,Copy_Log2Pass0} } ,
 { { 0 ,239 ,1*63 ,2*63+1} ,Invalid_Button ,1 ,{Rien     ,Rien ,Copy_Log2Pass1} } ,
 { { 0 ,239 ,2*63 ,3*63+1} ,Invalid_Button ,1 ,{Rien     ,Rien ,Copy_Log2Pass2} } ,
 { { 0 ,239 ,3*63 ,4*63+1} ,Invalid_Button ,1 ,{Rien     ,Rien ,Copy_Log2Pass3} } ,
 { { 0 ,239 ,4*63 ,5*63+1} ,Invalid_Button ,1 ,{Rien     ,Rien ,Copy_Log2Pass4} } ,
 { {0  ,0   ,0    ,0}      ,Any_Button     ,0 ,{Rien     ,Rien ,Rien}           } ,
};

struct Struct_Pic Log_Pic [] RODATA=
{
   {  {21 ,21+21 ,14+0*65 ,14+0*65+29} ,0 ,0 ,7 ,Log_Events  ,LOG_LENGTH-1 ,Log_Data[0]} ,
   {  {21 ,21+21 ,14+1*65 ,14+1*65+29} ,0 ,0 ,1 ,Rien_Events ,LOG_LENGTH-1 ,Log_Data[1]} ,
   {  {21 ,21+21 ,14+2*65 ,14+2*65+29} ,0 ,0 ,1 ,Rien_Events ,LOG_LENGTH-1 ,Log_Data[2]} ,
   {  {21 ,21+21 ,14+3*65 ,14+3*65+29} ,0 ,0 ,1 ,Rien_Events ,LOG_LENGTH-1 ,Log_Data[3]} ,
   {  {21 ,21+21 ,14+4*65 ,14+4*65+29} ,0 ,0 ,1 ,Rien_Events ,LOG_LENGTH-1 ,Log_Data[4]} ,

   {  { 0 ,239 ,0*63 ,0*63+1} ,0 ,1 ,1 ,Rien_Events ,1 ,Line_Green_Data },
   {  { 0 ,239 ,1*63 ,1*63+1} ,0 ,1 ,1 ,Rien_Events ,1 ,Line_Green_Data },
   {  { 0 ,239 ,2*63 ,2*63+1} ,0 ,1 ,1 ,Rien_Events ,1 ,Line_Green_Data },
   {  { 0 ,239 ,3*63 ,3*63+1} ,0 ,1 ,1 ,Rien_Events ,1 ,Line_Green_Data },
   {  { 0 ,239 ,4*63 ,4*63+1} ,0 ,1 ,1 ,Rien_Events ,1 ,Line_Green_Data },
   {  { 0 ,239 ,318  ,319}    ,0 ,1 ,1 ,Rien_Events ,1 ,Line_Green_Data },

   {  { 0   ,1   ,0 ,319} ,0 ,1 ,1 ,Rien_Events ,1 ,Line_Green_Data} ,
   {  { 238 ,239 ,0 ,319} ,0 ,1 ,1 ,Rien_Events ,1 ,Line_Green_Data} ,
};

void Log_String2Pic(void)
{
   uint8_t i;
   for(i=0;i<LOG_LINES;i++)
      String2Pic_Data ( RLog.Line[i] ,Log_Pic[i].Data ,&Font_22x30_Pic );
   Layer_Structure_Modified();
}
void Init_Log(void)
{
   Log_String2Pic();
}

void Add_Log ( void ) {
   uint8_t i;
   Add_Bkgd_Black();
   for(i=0;i<sizeof(Log_Pic)/sizeof(Log_Pic[0]);i++)
      Add_Pic_On_Top ( &Log_Pic[i] );
}
void Del_Log ( void ) {
   uint8_t i;
   Del_Bkgd_Black();
   for(i=0;i<sizeof(Log_Pic)/sizeof(Log_Pic[0]);i++)
      Del_Pic ( &Log_Pic[i] );
   Layer_Clr_Lcd();
}

