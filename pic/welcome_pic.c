 #include "display_pics.h"
#include "type_conversion.h"
#include "display_layers.h"
#include "events.h"
#include "stdint.h"
#include "buttons_pic.h"
#include "welcome_pic.h"
#include "serial_tx.h"

//--------------Pics Data----------------------------------
uint16_t Welcome_Data_Raw[] RODATA=
{
//	#include "green.raw"
	#include "welcome.raw"
};
uint16_t *Welcome_Data[] RODATA=
{
	Welcome_Data_Raw,
};

//---------------Functions---------------------------------------
void Welcome_Constr(void)
{
//	Add_Button0();
	//Add_Read();
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
 0,239,0,319,0,0,1,Welcome_Events,1,Welcome_Data
};
void Add_Welcome				(void)	{Add_Pic_On_Bottom(&Welcome_Pic);}
void Del_Welcome				(void)	{Del_Pic(&Welcome_Pic);}
//------------------------------------------------------
uint16_t Read_Data_Raw[1000] =
{
	0,	
};
uint16_t *Read_Data[] RODATA=
{
	Read_Data_Raw,
};
struct Struct_Pic Read_Pic=
{
 0,0+30,0,0+30,0,0,1,Rien_Events,1,Read_Data
};
void Add_Read				(void)	{Add_Pic_On_Top(&Read_Pic);}
void Del_Read				(void)	{Del_Pic(&Read_Pic);}
void Update_Read_Pic(void)
{
	Lcd2Pic(&Read_Pic);
	Layer_Structure_Modified();
}
void Move_Read_Pic(void)
{
	Read_Pic.Start_X+=10;
	Read_Pic.End_X+=10;
	Read_Pic.Start_Y+=10;
	Read_Pic.End_Y+=10;
}
	

//
