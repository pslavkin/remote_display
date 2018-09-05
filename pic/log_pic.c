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
//------------------------------------------------------
char Log_Table[][10]= {
   "11111111+",
   "22222222+",
   "33333333-",
   "44444444+",
   "55555555-",
};

uint16_t* Log_Data[sizeof(Log_Table)/sizeof(Log_Table[0])][10];
uint16_t Line_Raw[] RODATA=
{
   0x03E0,
};
uint16_t *Line_Data[] RODATA=
{
   Line_Raw,
};

void Copy_Pass2Log(char* Pass, bool Accepted)
{
   uint8_t i;
   for(i=sizeof(Log_Table)/sizeof(Log_Table[0])-1;i>0;i--)
      strcpy(Log_Table[i],Log_Table[i-1]);
   strcpy(Log_Table[0],Pass);
   Append_Data(Log_Table[0] ,Accepted?'+':'-' ,10);
   String_Padd(Log_Table[0] ,Log_Table[0]     ,10 ,' ');
}
void Copy_Log2Pass(uint8_t N)
{
   strncpy(Read_Pass_String(),Log_Table[N],8);
   Read_Pass_String()[strlen(Read_Pass_String())-1]='\0';
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
 { {0 ,0   ,0    ,0}    ,Invalid_Button ,0 ,{Init_Log ,Rien ,Rien}           },// On_Create
 { {0 ,239 ,0*65 ,1*65} ,Invalid_Button ,1 ,{Rien     ,Rien ,Copy_Log2Pass0} },
 { {0 ,239 ,1*65 ,2*65} ,Invalid_Button ,1 ,{Rien     ,Rien ,Copy_Log2Pass1} },
 { {0 ,239 ,2*65 ,3*65} ,Invalid_Button ,1 ,{Rien     ,Rien ,Copy_Log2Pass2} },
 { {0 ,239 ,3*65 ,4*65} ,Invalid_Button ,1 ,{Rien     ,Rien ,Copy_Log2Pass3} },
 { {0 ,239 ,4*65 ,5*65} ,Invalid_Button ,1 ,{Rien     ,Rien ,Copy_Log2Pass4} },
 { {0 ,0   ,0    ,0}    ,Any_Button     ,0 ,{Rien     ,Rien ,Rien}           },
};

struct Struct_Pic Log_Pic [] RODATA=
{
   {  {21 ,21+21 ,14+0*65 ,14+0*65+30} ,0 ,0 ,7 ,Log_Events  ,sizeof(Log_Table[0])-1 ,Log_Data[0]} ,
   {  {21 ,21+21 ,14+1*65 ,14+1*65+30} ,0 ,0 ,1 ,Rien_Events ,sizeof(Log_Table[0])-1 ,Log_Data[1]} ,
   {  {21 ,21+21 ,14+2*65 ,14+2*65+30} ,0 ,0 ,1 ,Rien_Events ,sizeof(Log_Table[0])-1 ,Log_Data[2]} ,
   {  {21 ,21+21 ,14+3*65 ,14+3*65+30} ,0 ,0 ,1 ,Rien_Events ,sizeof(Log_Table[0])-1 ,Log_Data[3]} ,
   {  {21 ,21+21 ,14+4*65 ,14+4*65+30} ,0 ,0 ,1 ,Rien_Events ,sizeof(Log_Table[0])-1 ,Log_Data[4]} ,

   {  { 0 , 239 , 1*63 ,1*63+1} ,0 ,1 ,1 ,Rien_Events ,1 ,Line_Data},
   {  { 0 , 239 , 2*63 ,2*63+1} ,0 ,1 ,1 ,Rien_Events ,1 ,Line_Data},
   {  { 0 , 239 , 3*63 ,3*63+1} ,0 ,1 ,1 ,Rien_Events ,1 ,Line_Data},
   {  { 0 , 239 , 4*63 ,4*63+1} ,0 ,1 ,1 ,Rien_Events ,1 ,Line_Data},

   {  { 0   ,1   ,0 ,319} ,0 ,1 ,1 ,Rien_Events ,1 ,Line_Data} ,
   {  { 238 ,239 ,0 ,319} ,0 ,1 ,1 ,Rien_Events ,1 ,Line_Data} ,
};

void Log_String2Pic(void)
{
   uint8_t i;
   for(i=0;i<sizeof(Log_Table)/sizeof(Log_Table[0]);i++)
      String2Pic_Data ( Log_Table[i] ,Log_Pic[i].Data ,&Font_22x30_Pic );
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

