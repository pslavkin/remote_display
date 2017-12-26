#include "display_pics.h"
#include "type_conversion.h"
#include "display_layers.h"
#include "events.h"

//--------------Pics Data----------------------------------
unsigned char Welcome_Data[] RODATA=
{
#include "calc3.txt"
};

//---------------Functions---------------------------------------
void Welcome_Constr(void)
{
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
 0,239,0,320,3,0,1,Welcome_Events,1,Welcome_Data
};
void Add_Welcome				(void)	{Add_Pic_On_Bottom(&Welcome_Pic);}
void Del_Welcome				(void)	{Del_Pic(&Welcome_Pic);}
//------------------------------------------------------
