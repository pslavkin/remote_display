#include "display_pics.h"
#include "type_conversion.h"
#include "display_layers.h"
#include "events.h"
#include "stdint.h"
#include "welcome_pic.h"
#include "serial_tx.h"
#include "mask_pic.h"
#include "numbers_pic.h"
#include "keyb_pic.h"

//--------------Pics Data----------------------------------
uint16_t Welcome_Data_Raw[] RODATA=
{
	#include "pointer_logo.raw"
};
uint16_t *Welcome_Data[] RODATA=
{
	Welcome_Data_Raw,
};

//---------------Functions---------------------------------------
void Welcome_Constr(void)
{
}
void Welcome_Destr(void)
{
	Add_Keyb();
}
//--------------Events----------------------------------
struct Struct_Pic_Events Welcome_Events[] RODATA=
{
{  0,  0,  0,  0, 	0,	{Welcome_Constr,		Del_Welcome,		Welcome_Destr},}			//On_Create
};
//--------------Pics Info----------------------------------
//
struct Struct_Pic Welcome_Pic RODATA=
{
 16,207,100,125,10,0,1,Welcome_Events,1,Welcome_Data
};
void Add_Welcome				(void)	{Add_Pic_On_Bottom(&Welcome_Pic);}
void Del_Welcome				(void)	{Del_Pic(&Welcome_Pic);}
void Clear_All_And_Add_Welcome			(void)	{Del_All_Layers();Clear_Lcd();Add_Welcome();}
//------------------------------------------------------
