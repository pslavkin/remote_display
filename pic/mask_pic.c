#include "display_pics.h"
#include "type_conversion.h"
#include "display_layers.h"
#include "stdint.h"
#include "mask_pic.h"

//------------------------------------------------------
uint16_t Mask_Raw[100*100] =
{
	0,	
};
uint16_t *Mask[] RODATA=
{
	Mask_Raw,
};
struct Struct_Pic Mask_Pic=
{
 0,0,0,0,0,0,1,Rien_Events,1,Mask
};

void Update_Mask_Pic(void)
{
	Lcd2Pic_Inverted(&Mask_Pic);
}
struct Struct_Pic* Read_Mask_Pic(void)
{
	return &Mask_Pic;
}
void Resize_Mask_Pic(uint16_t Start_X,uint16_t End_X,uint16_t Start_Y,uint16_t End_Y)
{
	Mask_Pic.Start_X=Start_X;
	Mask_Pic.End_X  =End_X;
	Mask_Pic.Start_Y=Start_Y;
	Mask_Pic.End_Y  =End_Y;
	Layer_Structure_Modified();
}
void Add_Mask_Delete(void) 	{Resize_Mask_Pic(  4,  4+73,  5,  5+53);}
void Add_Mask0(void) 		{Resize_Mask_Pic( 83, 83+73,  5,  5+53);}
void Add_Mask_Enter(void) 	{Resize_Mask_Pic(161,161+73,  5,  5+53);}
void Add_Mask1(void) 		{Resize_Mask_Pic(  4,  4+73,183,183+53);}
void Add_Mask2(void) 		{Resize_Mask_Pic( 83, 83+73,183,183+53);}
void Add_Mask3(void) 		{Resize_Mask_Pic(161,161+73,183,183+53);}
void Add_Mask4(void) 		{Resize_Mask_Pic(  4,  4+73,124,124+53);}
void Add_Mask5(void) 		{Resize_Mask_Pic( 83, 83+73,124,124+53);}
void Add_Mask6(void) 		{Resize_Mask_Pic(161,161+73,124,124+53);}
void Add_Mask7(void) 		{Resize_Mask_Pic(  4,  4+73, 64, 64+53);}
void Add_Mask8(void) 		{Resize_Mask_Pic( 83, 83+73, 64, 64+53);}
void Add_Mask9(void) 		{Resize_Mask_Pic(161,161+73, 64, 64+53);}
void Del_Mask(void) 		{Resize_Mask_Pic(  0,  0+ 0,  0,  0+ 0);}

