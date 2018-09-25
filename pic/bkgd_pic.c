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
//------------RED-------------------------------------
//--------------Pics Data----------------------------------
uint16_t Bkgd_Red_Data_Raw[] RODATA=
{
#ifdef PICS_ENABLED
   0xF800
#endif
};
uint16_t *Bkgd_Red_Data[] RODATA=
{
   Bkgd_Red_Data_Raw,
};
//--------------Events----------------------------------
struct Struct_Pic_Events Bkgd_Red_Events[] RODATA=
{
{ { 0 ,0 ,0 ,0 } ,0 ,0 ,{Rien ,Rien ,Rien}} ,// On_Create
};
//--------------Pics Info----------------------------------
struct Struct_Pic Bkgd_Red_Pic RODATA=
{
   { 0 ,239 ,0 ,319 } ,0 ,1 ,1 ,Bkgd_Red_Events ,1 ,Bkgd_Red_Data
};
void Add_Bkgd_Red ( void ) { Add_Pic_On_Bottom(&Bkgd_Red_Pic);}
void Del_Bkgd_Red ( void ) { Del_Pic(&Bkgd_Red_Pic)       ;}
//------------GREEN-------------------------------------
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
{ { 0 ,0 ,0 ,0 } ,0 ,0 ,{Rien ,Rien ,Rien}} ,// On_Create
};
//--------------Pics Info----------------------------------
struct Struct_Pic Bkgd_Green_Pic RODATA=
{
   { 0 ,239 , 0 ,319 },0 ,1 ,1 ,Bkgd_Green_Events ,1 ,Bkgd_Green_Data
};
void Add_Bkgd_Green ( void ) { Add_Pic_On_Bottom(&Bkgd_Green_Pic);}
void Del_Bkgd_Green ( void ) { Del_Pic(&Bkgd_Green_Pic)          ;}
//---------------BLUE-----------------------------------
//--------------Pics Data----------------------------------
uint16_t Bkgd_Blue_Data_Raw[] RODATA=
{
#ifdef PICS_ENABLED
   0x001F
#endif
};
uint16_t *Bkgd_Blue_Data[] RODATA=
{
   Bkgd_Blue_Data_Raw,
};
//--------------Events----------------------------------
struct Struct_Pic_Events Bkgd_Blue_Events[] RODATA=
{
{ { 0 ,0 ,0 ,0 } ,0 ,0 ,{Rien ,Rien ,Rien}} ,// On_Create
};
//--------------Pics Info----------------------------------
struct Struct_Pic Bkgd_Blue_Pic RODATA=
{
   { 0 ,239 , 0 ,319 },0 ,1 ,1 ,Bkgd_Blue_Events ,1 ,Bkgd_Blue_Data
};
void Add_Bkgd_Blue ( void ) { Add_Pic_On_Bottom(&Bkgd_Blue_Pic);}
void Del_Bkgd_Blue ( void ) { Del_Pic(&Bkgd_Blue_Pic)       ;}

//---------------YELLOW-----------------------------------
//--------------Pics Data----------------------------------
uint16_t Bkgd_Yellow_Data_Raw[] RODATA=
{
#ifdef PICS_ENABLED
   0xFFE0
#endif
};
uint16_t *Bkgd_Yellow_Data[] RODATA=
{
   Bkgd_Yellow_Data_Raw,
};
//--------------Events----------------------------------
struct Struct_Pic_Events Bkgd_Yellow_Events[] RODATA=
{
{ { 0 ,0 ,0 ,0 } ,0 ,0 ,{Rien ,Rien ,Rien}} ,// On_Create
};
//--------------Pics Info----------------------------------
struct Struct_Pic Bkgd_Yellow_Pic RODATA=
{
   { 0 ,239 , 0 ,319 },0 ,1 ,1 ,Bkgd_Yellow_Events ,1 ,Bkgd_Yellow_Data
};
void Add_Bkgd_Yellow ( void ) { Add_Pic_On_Bottom(&Bkgd_Yellow_Pic);}
void Del_Bkgd_Yellow ( void ) { Del_Pic(&Bkgd_Yellow_Pic)       ;}

//--------------LINE GREEN DATA---------------------------------------------
uint16_t Line_Green_Raw[] RODATA=
{
   0x03E0,
};
uint16_t *Line_Green_Data[] RODATA=
{
   Line_Green_Raw,
};
//--------------LINE BLUE DATA---------------------------------------------
uint16_t Line_Blue_Raw[] RODATA=
{
   0x001F,
};
uint16_t *Line_Blue_Data[] RODATA=
{
   Line_Blue_Raw,
};
//--------------LINE BLUE DATA---------------------------------------------
uint16_t Line_White_Raw[] RODATA=
{
   0xFFFF,
};
uint16_t *Line_White_Data[] RODATA=
{
   Line_White_Raw,
};
//------------------------------------------------------------------------------
