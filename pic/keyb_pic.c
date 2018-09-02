#include "display_pics.h"
#include "type_conversion.h"
#include "display_layers.h"
#include "events.h"
#include "stdint.h"
#include "keyb_pic.h"
#include "serial_tx.h"
#include "mask_pic.h"
#include "numbers_pic.h"
#include "debug.h"

//--------------Pics Data----------------------------------
uint16_t Keyb_Data_Raw[] RODATA=
{
#ifdef PICS_ENABLED
   #include "keyb.raw"
#endif
};
uint16_t *Keyb_Data[] RODATA=
{
   Keyb_Data_Raw,
};

//---------------Functions---------------------------------------
void Keyb_Constr(void)
{
   Add_Numbers();
}
void Keyb_Destr(void)
{
}
//--------------Events----------------------------------
struct Struct_Pic_Events Keyb_Events[] RODATA=
{
{  0,  0,  0,  0,    0, {Keyb_Constr,     Rien,    Keyb_Destr},}        //On_Create
};
//--------------Pics Info----------------------------------
struct Struct_Pic Keyb_Pic RODATA=
{
 0,239,0,319,0,0,1,Keyb_Events,1,Keyb_Data
};
void Add_Keyb ( void ) { Add_Pic_On_Bottom(&Keyb_Pic);}
void Del_Keyb ( void ) { Del_Pic(&Keyb_Pic)          ;}
//------------------------------------------------------
