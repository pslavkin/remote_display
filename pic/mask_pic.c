#include "display_pics.h"
#include "type_conversion.h"
#include "display_layers.h"
#include "stdint.h"
#include "schedule.h"
#include "mask_pic.h"
#include <stdint.h>
#include "accept_reject_pic.h"
#include <string.h>

//---------------Functions---------------------------------------
struct Struct_Pic Mask_Pic=
{
   {0,0,0,0},0,0,1,Rien_Events,1,NULL
};
void Update_Mask_Pic(void)
{
   Invert_Pic(&Mask_Pic);
}
void Set_Mask_Pic(struct Struct_Pic_Pos *Pos)
{
   if(!Pic_Pos_Compare(&Mask_Pic.Pos,Pos)) {      //si no cambio para que voy a hinchar las guindas...
      Mask_Pic.Pos=*Pos;
      Layer_Structure_Modified();
   };
}

