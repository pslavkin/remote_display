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
//------------------------------------------------------
char Log_Table[][10]= {
   "11111111+",
   "22222222+",
   "33333333-",
   "44444444+",
   "55555555+",
};
uint16_t* Log_Data[sizeof(Log_Table)/sizeof(Log_Table[0])][10];


struct Struct_Pic_Events Log_Events[] =
{
 {{0  ,0       ,0       ,0}          ,Invalid_Button ,0 ,{Init_Log ,Del_Log ,Rien}} ,// On_Create
 {{0 ,239 ,0*65,1*65} ,Invalid_Button ,1 ,{Rien     ,Rien    ,Rien}} ,// On_Create
 {{0 ,239 ,1*65,2*65} ,Invalid_Button ,1 ,{Rien     ,Rien    ,Rien}} ,// On_Create
 {{0 ,239 ,2*65,3*65} ,Invalid_Button ,1 ,{Rien     ,Rien    ,Rien}} ,// On_Create
 {{0 ,239 ,3*65,4*65} ,Invalid_Button ,1 ,{Rien     ,Rien    ,Rien}} ,// On_Create
 {{0 ,239 ,4*65,5*65} ,Invalid_Button ,1 ,{Rien     ,Rien    ,Rien}} ,// On_Create
// {{0 ,0     ,0       ,0}          ,Any_Button     ,0 ,{Rien     ,Rien    ,Rien}} ,// On_Create
};

struct Struct_Pic Log_Pic [] RODATA=
{
   {  {21 ,21+21 ,14+0*65 ,14+0*65+30} ,10 ,0 ,6 ,Log_Events  ,sizeof(Log_Table[0])-1 ,Log_Data[0]},
   {  {21 ,21+21 ,14+1*65 ,14+1*65+30} ,0  ,0 ,1 ,Rien_Events ,sizeof(Log_Table[0])-1 ,Log_Data[1]},
   {  {21 ,21+21 ,14+2*65 ,14+2*65+30} ,0  ,0 ,1 ,Rien_Events ,sizeof(Log_Table[0])-1 ,Log_Data[2]},
   {  {21 ,21+21 ,14+3*65 ,14+3*65+30} ,0  ,0 ,1 ,Rien_Events ,sizeof(Log_Table[0])-1 ,Log_Data[3]},
   {  {21 ,21+21 ,14+4*65 ,14+4*65+30} ,0  ,0 ,1 ,Rien_Events ,sizeof(Log_Table[0])-1 ,Log_Data[4]},
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
   Add_Bkgd_Black();
   uint8_t i;
   for(i=0;i<sizeof(Log_Table)/sizeof(Log_Table[0]);i++)
      Add_Pic_On_Top ( &Log_Pic[i] );
}
void Del_Log ( void ) {
   Del_Bkgd_Black();
   uint8_t i;
   for(i=0;i<sizeof(Log_Table)/sizeof(Log_Table[0]);i++)
      Del_Pic ( &Log_Pic[i] );
   Layer_Clr_Lcd();
}

