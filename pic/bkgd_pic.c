#include "display_pics.h"
#include "type_conversion.h"
#include "display_layers.h"
#include "events.h"
#include "bkgd_pic.h"
#include "debug.h"
#include "tpanel.h"

//--------------Pics Data----------------------------------
uint16_t Bkgd_Black_Data_Raw[] RODATA=
{
#ifdef PICS_ENABLED
   0x0000,
#endif
};
uint16_t *Bkgd_Black_Data[] RODATA=
{
   Bkgd_Black_Data_Raw,
};
//--------------Events----------------------------------
struct Struct_Pic_Events Bkgd_Black_Events[] RODATA=
{
{ { 0 ,0 ,0 ,0 } ,0          ,0 ,{Rien ,Rien ,Rien} },// On_Create
{ { 0 ,0 ,0 ,0 } ,Any_Button ,0 ,{Rien ,Rien ,Rien} },// no solo tapa la imagen sino tambien los eventos mas abajo de esto
};
//--------------Pics Info----------------------------------
struct Struct_Pic Bkgd_Black_Pic RODATA=
{
   { 0 ,239 , 0 ,319 },0 ,1 ,2 ,Bkgd_Black_Events ,1 ,Bkgd_Black_Data
};
void Add_Bkgd_Black                    ( void ) 
{ 
   Set_Watermark_Layer (                 );
   Add_Pic_On_Top      ( &Bkgd_Black_Pic );
}
void Del_Bkgd_Black                    ( void ) {
   Del_Pic               ( &Bkgd_Black_Pic );
   Reset_Watermark_Layer (                 );
}
struct Struct_Pic* Read_Bkgd_Black_Pic ( void ) { return &Bkgd_Black_Pic            ;}
//------------------------------------------------------
//--------------Pics Data----------------------------------
uint16_t Bkgd_Green_Data_Raw[] RODATA=
{
#ifdef PICS_ENABLED
   0x07E0
#endif
};
uint16_t *Bkgd_Green_Data[] RODATA=
{
   Bkgd_Green_Data_Raw,
};
//--------------Events----------------------------------
struct Struct_Pic_Events Bkgd_Green_Events[] RODATA=
{
{ { 0 ,0 ,0 ,0 } ,0          ,0 ,{Rien ,Rien ,Rien}} ,// On_Create
};
//--------------Pics Info----------------------------------
struct Struct_Pic Bkgd_Green_Pic RODATA=
{
   { 0 ,239 , 0 ,319 },0 ,1 ,1 ,Bkgd_Green_Events ,1 ,Bkgd_Green_Data
};
void Add_Bkgd_Green ( void ) { Add_Pic_On_Bottom(&Bkgd_Green_Pic);}
void Del_Bkgd_Green ( void ) { Del_Pic(&Bkgd_Green_Pic)       ;}
//------------------------------------------------------
