#include "display_pics.h"
#include "type_conversion.h"
#include "display_layers.h"
#include "events.h"
#include "stdint.h"
#include "welcome_pic.h"
#include "serial_tx.h"
#include "mask_pic.h"
#include "pass_pic.h"
#include "keyb_pic.h"
#include "debug.h"
#include "schedule.h"

//--------------Pics Data----------------------------------
uint16_t Welcome_Data_Raw[] RODATA=
{
#ifdef PICS_ENABLED
//#ifdef PICS_ENABLED_TEST
   #include "pointer_logo.raw"
#endif
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
   {{ 0 ,0 ,0 ,0} ,0 ,0 ,{Welcome_Constr ,Del_Welcome ,Welcome_Destr} ,} // On_Create
};
//--------------Pics Info----------------------------------
//
struct Struct_Pic Welcome_Pic RODATA=
{
   {16,16+207,100,100+125},4,0,1,Welcome_Events,1,Welcome_Data
};
void Add_Welcome               ( void ) { Add_Pic_On_Bottom(&Welcome_Pic);}
void Del_Welcome               ( void ) { Del_Pic(&Welcome_Pic)          ;}
void Clear_All_And_Add_Welcome ( void ) { Del_All_Layers()               ;Clear_Lcd();Add_Welcome();}
//--------------test Info----------------------------------
uint16_t Test_Pic_Data_Raw[] RODATA=
{
#ifdef PICS_ENABLED_TEST
   0xF800,0x7E0,0x01F,0x1234,0x4444,0x6666,0x0000,0x0000

//#include "colors.raw"
#endif
};
uint16_t *Test_Pic_Data[] RODATA=
{
   Test_Pic_Data_Raw,
};

void Test_Pic_Constr(void)
{
}
void Test_Pic_Destr(void)
{
//   New_Periodic_Func_Schedule(2,Clear_Lcd);
//   New_None_Periodic_Func_Schedule(2,Add_Test_Pic);
}
//--------------Events----------------------------------
struct Struct_Pic_Events Test_Pic_Events[] RODATA=
{
{ {0 ,0 ,0 ,0 },0 , 0, {Test_Pic_Constr ,Del_Test_Pic ,Test_Pic_Destr} ,} // On_Create
};
//--------------Pics Info----------------------------------
//
struct Struct_Pic Test_Pic_Pic RODATA=
{
   {0,100,0,100},0,8,1,Test_Pic_Events,1,Test_Pic_Data
};
void Add_Test_Pic               ( void ) { Add_Pic_On_Bottom(&Test_Pic_Pic);}
void Del_Test_Pic               ( void ) { Del_Pic(&Test_Pic_Pic)          ;}
//------------------------------------------------------
