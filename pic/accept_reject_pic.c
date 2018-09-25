#include "display_pics.h"
#include "type_conversion.h"
#include "display_layers.h"
#include "events.h"
#include "stdint.h"
#include "accept_reject_pic.h"
#include "serial_tx.h"
#include "mask_pic.h"
#include "pass_pic.h"
#include "pass_big_pic.h"
#include "debug.h"
#include "tpanel.h"
#include "bkgd_pic.h"
#include "log_pic.h"
#include "everythings.h"
#include "flash.h"
#include "str.h"
#include "clock_pic.h"
#include "one_wire.h"

//--------------Pics Data----------------------------------
uint16_t Accept_Data_Raw[] RODATA=
{
#ifdef PICS_ENABLED
   #include "accepted.raw"
#endif
};
uint16_t *Accept_Data[] RODATA=
{
   Accept_Data_Raw,
};

//---------------Functions---------------------------------------
void Accept_Constr(void)
{
   Copy_Pass2Log  ( Read_Pass_String( ),true);
   Send_NVData2Serial(10,"Accepted\r\n");
//   Save_Table_Log (                   )      ; //no quiere que grabe cuando apaga!!
}
void Accept_Destr(void)
{
  Del_Bkgd_Black ( );
  Add_Pass_Big   ( );
  Layer_Clr_Lcd  ( );
}
//--------------Events----------------------------------
struct Struct_Pic_Events Accept_Events[] RODATA=
{
{ { 0  ,0   ,0  ,0 }   ,0          ,0 ,{Accept_Constr ,Del_Accept ,Accept_Destr} }       ,// On_Create
{ { 26 ,212 ,66 ,252 } ,Any_Button ,0 ,{Rien          ,Rien       ,Del_Accept}         } ,
};
//--------------Pics Info----------------------------------
//
struct Struct_Pic Accept_Pic RODATA=
{
   { 26 ,212 ,66 ,252 },3 ,0 ,2 ,Accept_Events ,1 ,Accept_Data
};
void Add_Accept ( void )
{
   Add_Bkgd_Black (             );
   Add_Pic_On_Top ( &Accept_Pic );
}
void Del_Accept ( void ) { Del_Pic(&Accept_Pic)       ;}
//------------------------------------------------------
//--------------Pics Data----------------------------------
uint16_t Reject_Data_Raw[] RODATA=
{
#ifdef PICS_ENABLED
   #include "rejected.raw"
#endif
};
uint16_t *Reject_Data[] RODATA=
{
   Reject_Data_Raw,
};
//---------------Functions---------------------------------------
void Reject_Constr(void)
{
   Copy_Pass2Log(Read_Pass_String(),false);
   Send_NVData2Serial(10,"Rejected\r\n");
//   Save_Table_Log (                   )      ; //no quiere que grabe cuando apaga!!
}
void Reject_Destr(void)
{
   Del_Bkgd_Black ( );
   Layer_Clr_Lcd  ( );
}
//--------------Events----------------------------------
struct Struct_Pic_Events Reject_Events[] RODATA=
{
   { { 0  ,0   ,0  ,0 }   ,0          ,0 ,{Reject_Constr ,Del_Reject ,Reject_Destr} } ,// On_Create
   { { 26 ,212 ,66 ,252 } ,Any_Button ,0 ,{Rien          ,Rien       ,Del_Reject }}   ,
};
//--------------Pics Info----------------------------------
//
struct Struct_Pic Reject_Pic RODATA=
{
   { 26 ,212 ,66 ,252 },5,0,2,Reject_Events,1,Reject_Data
};
void Add_Reject            (void)   {
   Add_Bkgd_Black (             );
   Add_Pic_On_Top ( &Reject_Pic );
}
void Del_Reject            (void)   {Del_Pic(&Reject_Pic);}
//--------------Pics Data----------------------------------
