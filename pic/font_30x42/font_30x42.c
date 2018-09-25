#include "display_phisical.h"
#include "display_layers.h"
#include "display_pics.h"
#include "type_conversion.h"
#include "display_layers.h"
#include "stdint.h"
#include "pass_pic.h"
#include "debug.h"
#include "font_80x88.h"
#include <string.h>
//-----------------------------------------------------
uint16_t Font_30x42_0_Raw[] RODATA= {/*{{{*/
#ifdef PICS_ENABLED
   #include "font_30x42_0.raw"
#endif
};
uint16_t Font_30x42_1_Raw[] RODATA= {
#ifdef PICS_ENABLED
   #include "font_30x42_1.raw"
#endif
};
uint16_t Font_30x42_2_Raw[] RODATA= {
#ifdef PICS_ENABLED
   #include "font_30x42_2.raw"
#endif
};
uint16_t Font_30x42_3_Raw[] RODATA= {
#ifdef PICS_ENABLED
   #include "font_30x42_3.raw"
#endif
};
uint16_t Font_30x42_4_Raw[] RODATA= {
#ifdef PICS_ENABLED
   #include "font_30x42_4.raw"
#endif
};
uint16_t Font_30x42_5_Raw[] RODATA= {
#ifdef PICS_ENABLED
   #include "font_30x42_5.raw"
#endif
};
uint16_t Font_30x42_6_Raw[] RODATA= {
#ifdef PICS_ENABLED
   #include "font_30x42_6.raw"
#endif
};
uint16_t Font_30x42_7_Raw[] RODATA= {
#ifdef PICS_ENABLED
   #include "font_30x42_7.raw"
#endif
};
uint16_t Font_30x42_8_Raw[] RODATA= {
#ifdef PICS_ENABLED
   #include "font_30x42_8.raw"
#endif
};
uint16_t Font_30x42_9_Raw[] RODATA= {
#ifdef PICS_ENABLED
   #include "font_30x42_9.raw"
#endif
};
uint16_t Font_30x42__Raw[] RODATA= {
#ifdef PICS_ENABLED
   #include "font_30x42__.raw"
#endif
};
//uint16_t Font_30x42_Space_Raw[] RODATA= {
//#ifdef PICS_ENABLED
//   #include "font_30x42_space.raw"
//#endif
//};
/*}}}*/

uint16_t * Font_30x42_Data[] RODATA=
{
Font_80x88_Space_Raw , //
//Font_30x42_Space_Raw , //
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
NULL, //+
NULL, //,
Font_30x42__Raw , //-
NULL, //.
NULL, ///
Font_30x42_0_Raw ,
Font_30x42_1_Raw ,
Font_30x42_2_Raw ,
Font_30x42_3_Raw ,
Font_30x42_4_Raw ,
Font_30x42_5_Raw ,
Font_30x42_6_Raw ,
Font_30x42_7_Raw ,
Font_30x42_8_Raw ,
Font_30x42_9_Raw ,
};

struct Struct_Pic Font_30x42_Pic RODATA={{0,29,0,41} ,0 ,0,0,Rien_Events ,1 ,Font_30x42_Data};
//----------------------------------
