#include "display_pics.h"
#include "type_conversion.h"
#include "display_layers.h"
#include "events.h"
#include "stdint.h"
#include "welcome_pic.h"
#include "serial_tx.h"
#include "mask_pic.h"
#include "numbers_pic.h"

//--------------Pics Data----------------------------------
uint16_t Welcome_Data_Raw[] RODATA=
{
	#include "welcome.raw"
};
uint16_t *Welcome_Data[] RODATA=
{
	Welcome_Data_Raw,
};

//---------------Functions---------------------------------------
void Welcome_Constr(void)
{
	Add_Numbers();
}
void Welcome_Destr(void)
{
}
//--------------Events----------------------------------
struct Struct_Pic_Events Welcome_Events[] RODATA=
{
{  0,  0,  0,  0, 	0,	{Welcome_Constr,		Rien,		Welcome_Destr},}			//On_Create
};
//--------------Pics Info----------------------------------
//
struct Struct_Pic Welcome_Pic RODATA=
{
 0,240,0,320,0,0,1,Welcome_Events,1,Welcome_Data
};
void Add_Welcome				(void)	{Add_Pic_On_Bottom(&Welcome_Pic);}
void Del_Welcome				(void)	{Del_Pic(&Welcome_Pic);}
//------------------------------------------------------
