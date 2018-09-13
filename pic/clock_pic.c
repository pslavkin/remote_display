#include "display_pics.h"
#include "type_conversion.h"
#include "display_layers.h"
#include "events.h"
#include "stdint.h"
#include "clock_pic.h"
#include "serial_tx.h"
#include "mask_pic.h"
#include "pass_pic.h"
#include "debug.h"
#include "tpanel.h"
#include "bkgd_pic.h"
#include "log_pic.h"
#include "everythings.h"
#include "flash.h"
#include "str.h"
#include "one_wire.h"
#include "accept_reject_pic.h"

uint8_t Time;
//--------------Pics Data----------------------------------
uint16_t Clock_Data_Raw[] RODATA=
{
#ifdef PICS_ENABLED
   #include "clock.raw"
#endif
};
uint16_t *Clock_Data[] RODATA=
{
   Clock_Data_Raw,
};
//---------------Functions---------------------------------------
void Clock_Constr(void)
{
   Add_Arena_Up();
}
void Clock_Destr(void)
{
   Del_Bkgd_Black ( );
   Layer_Clr_Lcd  ( );
}
//--------------Events----------------------------------
struct Struct_Pic_Events Clock_Events[] RODATA=
{
   { { 0 ,0 ,0 ,0 } ,0 ,0 ,{Clock_Constr ,Rien ,Clock_Destr} } ,// On_Create
};
//--------------Pics Info----------------------------------
struct Struct_Pic Clock_Pic RODATA=
{
   { 67 ,170 ,66 ,252 },0,0,1,Clock_Events,1,Clock_Data
};
void Add_Clock            (void)
{
   Add_Bkgd_Black (            );
   Add_Pic_On_Top ( &Clock_Pic );
}
void Del_Clock ( void ) { Del_Pic        ( &Clock_Pic );}
//--------------Pics Data----------------------------------
uint16_t Arena_Up_Data_Raw[] RODATA=
{
#ifdef PICS_ENABLED
   #include "arena_up.raw"
#endif
};
uint16_t *Arena_Up_Data[] RODATA=
{
   Arena_Up_Data_Raw,
};
//---------------Functions---------------------------------------
void Arena_Up_Constr(void)
{
   char Buf[13];
   String_Head(Read_Pass_String(),Buf,13,'0');
   Write_New_Code((uint8_t*)Buf);
   Time=0;
}
void Arena_Up_Destr(void)
{
}
void Test_One_Wire_Status(void)
{
   Send_NVData2Serial(22,"Test One Wire Status\r\n");
   if(Time<10) {
      Time++;
      if(One_Wire_Busy()==false) {
         Del_Arena_Up   ( );
         Add_Arena_Down ( );
         Send_NVData2Serial(11,"Busy Free\r\n");
      }
   }
   else {
      Send_One_Wire_Abort_Event ( );
      Del_Arena_Up              ( );
      Del_Clock                 ( );
      Add_Reject                ( );
//      Layer_Clr_Lcd             ( );
   }
}
//--------------Events----------------------------------
struct Struct_Pic_Events Arena_Up_Events[] RODATA=
{
   { { 0 ,0 ,0 ,0 } ,0 ,0 ,{Arena_Up_Constr ,Test_One_Wire_Status ,Arena_Up_Destr} } ,// On_Create
};
//--------------Pics Info----------------------------------
struct Struct_Pic Arena_Up_Pic RODATA=
{
   { 67+14 , 67+14+75 ,66+25 , 66+25+43 },1,0,1,Arena_Up_Events,1,Arena_Up_Data
};
void Add_Arena_Up ( void ) { Add_Pic_On_Top ( &Arena_Up_Pic );}
void Del_Arena_Up ( void ) { Del_Pic        ( &Arena_Up_Pic );}
//--------------Pics Data----------------------------------
uint16_t Arena_Down_Data_Raw[] RODATA=
{
#ifdef PICS_ENABLED
   #include "arena_down.raw"
#endif
};
uint16_t *Arena_Down_Data[] RODATA=
{
   Arena_Down_Data_Raw,
};
//---------------Functions---------------------------------------
void Arena_Down_Constr(void)
{
   Time=0;
}
void Arena_Down_Destr(void)
{
}
void Test_Ack_Pin(void)
{
   Send_NVData2Serial(14,"Test Ack pin\r\n");
   if(Time<10) {
      Time++;
      if(Read_Ack_Pin()==0) {
         Del_Arena_Down ( );
         Del_Clock      ( );
         Add_Accept     ( );
//         Layer_Clr_Lcd  ( );
      }
   }
   else {
         Del_Arena_Down ( );
         Del_Clock      ( );
         Add_Reject     ( );
 //        Layer_Clr_Lcd  ( );
   }
}
//--------------Events----------------------------------
struct Struct_Pic_Events Arena_Down_Events[] RODATA=
{
   { { 0 ,0 ,0 ,0 } ,0 ,0 ,{Arena_Down_Constr ,Test_Ack_Pin ,Arena_Down_Destr} } ,// On_Create
};
//--------------Pics Info----------------------------------
struct Struct_Pic Arena_Down_Pic RODATA=
{
   { 67 +14, 67+14+74 ,66+127 ,66+127+30 },1,0,1,Arena_Down_Events,1,Arena_Down_Data
};
void Add_Arena_Down ( void ) { Add_Pic_On_Top ( &Arena_Down_Pic );}
void Del_Arena_Down ( void ) { Del_Pic        ( &Arena_Down_Pic );}
//------------------------------------------------------
