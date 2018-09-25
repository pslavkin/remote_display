#include "display_phisical.h"
#include "display_layers.h"
#include "display_pics.h"
#include "type_conversion.h"
#include "display_layers.h"
#include "stdint.h"
#include "pass_pic.h"
#include "debug.h"
#include <string.h>
//-----------------------------------------------------
uint16_t Font_80x88_0_Raw[] RODATA= {/*{{{*/
#ifdef PICS_ENABLED
   #include "font_80x88_0.raw"
#endif
};
uint16_t Font_80x88_1_Raw[] RODATA= {
#ifdef PICS_ENABLED
   #include "font_80x88_1.raw"
#endif
};
uint16_t Font_80x88_2_Raw[] RODATA= {
#ifdef PICS_ENABLED
   #include "font_80x88_2.raw"
#endif
};
uint16_t Font_80x88_3_Raw[] RODATA= {
#ifdef PICS_ENABLED
   #include "font_80x88_3.raw"
#endif
};
uint16_t Font_80x88_4_Raw[] RODATA= {
#ifdef PICS_ENABLED
   #include "font_80x88_4.raw"
#endif
};
uint16_t Font_80x88_5_Raw[] RODATA= {
#ifdef PICS_ENABLED
   #include "font_80x88_5.raw"
#endif
};
uint16_t Font_80x88_6_Raw[] RODATA= {
#ifdef PICS_ENABLED
   #include "font_80x88_6.raw"
#endif
};
uint16_t Font_80x88_7_Raw[] RODATA= {
#ifdef PICS_ENABLED
   #include "font_80x88_7.raw" 
#endif
};
uint16_t Font_80x88_8_Raw[] RODATA= {
#ifdef PICS_ENABLED
   #include "font_80x88_8.raw"
#endif
};
uint16_t Font_80x88_9_Raw[] RODATA= {
#ifdef PICS_ENABLED
   #include "font_80x88_9.raw"
#endif
};
uint16_t Font_80x88_Space_Raw[] RODATA= {
#ifdef PICS_ENABLED
   #include "font_80x88_space.raw"
#endif
};
uint16_t Font_80x88_Arrow_Raw[] RODATA= {
#ifdef PICS_ENABLED
   #include "font_80x88_arrow.raw"
#endif
};/*}}}*/

uint16_t * Font_80x88_Data[] RODATA=
{
Font_80x88_Space_Raw , //
NULL, //!
NULL, //"
NULL, //#
NULL, //$
NULL, //%
NULL, //&
NULL, //'
NULL, //(
NULL, //)
NULL, //*
Font_80x88_Arrow_Raw ,// NULL, //+
NULL, //,
NULL, //-
NULL, //.
NULL, ///
Font_80x88_0_Raw ,
Font_80x88_1_Raw ,
Font_80x88_2_Raw ,
Font_80x88_3_Raw ,
Font_80x88_4_Raw ,
Font_80x88_5_Raw ,
Font_80x88_6_Raw ,
Font_80x88_7_Raw ,
Font_80x88_8_Raw ,
Font_80x88_9_Raw ,
};

struct Struct_Pic Font_80x88_Pic RODATA={{0,79,0,87} ,0 ,0,0,Rien_Events ,1 ,Font_80x88_Data};
//----------------------------------
