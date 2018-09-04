#include "state_machine.h"
#include "events.h"
#include "display_phisical.h"
#include "display_layers.h"
#include "display_pics.h"
#include "type_conversion.h"
#include "everythings.h"
#include "str.h"

//-------------------------------------------------------------------------------------

struct Struct_Pic_Events Rien_Events[] RODATA =
{
{{0,0,0,0},0,0,{Rien,Rien,Rien},}
};
//--------------------------Tipografias---------------------------------------------------------
//--------------------------Pabtallas---------------------------------------------------------
//-------------------------------------------------------------------------------------
uint16_t Pic_Width(struct Struct_Pic *Pic)
{
   return Pic->Pos.XR-Pic->Pos.XL+1;
}
uint16_t Pic_Hight(struct Struct_Pic *Pic)
{
   return Pic->Pos.YL-Pic->Pos.YU+1;
}
uint32_t Pic_Area(struct Struct_Pic *Pic)
{
   return Pic_Width(Pic)*Pic_Hight(Pic);
}
bool Pic_Pos_Compare(struct Struct_Pic_Pos *A, struct Struct_Pic_Pos *B)
{
   return A->XL==B->XL && A->XR==B->XR && A->YU==B->YU && A->YL==B->YL;
}
void Init_Display_Pics(void)
{
}
uint8_t  String2Pic_Data ( char *String ,uint16_t** Data ,struct Struct_Pic *Font )
{
   uint8_t i;
   for(i=0;String[i]!='\0';i++)
      Data[i]=Font->Data[String[i]-' '];
   return i;
}
void String2Pic ( char *String ,struct Struct_Pic *Pic ,struct Struct_Pic *Font )
{
   Pic->PCount=String2Pic_Data(String,Pic->Data,Font);
   Layer_Structure_Modified();
}

