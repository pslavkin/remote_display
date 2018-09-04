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
uint16_t Font_22x30_Tilde_Raw[] RODATA= {/*{{{*/
#ifdef PICS_ENABLED_TEST
   #include "font_22x30_tilde.raw"
#endif
};
uint16_t Font_22x30_Cross_Raw[] RODATA= {
#ifdef PICS_ENABLED_TEST
   #include "font_22x30_cross.raw"
#endif
};
uint16_t Font_22x30_0_Raw[] RODATA= {
#ifdef PICS_ENABLED_TEST
   #include "font_22x30_0.raw"
#endif
};
uint16_t Font_22x30_1_Raw[] RODATA= {
#ifdef PICS_ENABLED_TEST
   #include "font_22x30_1.raw"
#endif
};
uint16_t Font_22x30_2_Raw[] RODATA= {
#ifdef PICS_ENABLED_TEST
   #include "font_22x30_2.raw"
#endif
};
uint16_t Font_22x30_3_Raw[] RODATA= {
#ifdef PICS_ENABLED_TEST
   #include "font_22x30_3.raw"
#endif
};
uint16_t Font_22x30_4_Raw[] RODATA= { 
#ifdef PICS_ENABLED_TEST
   #include "font_22x30_4.raw" 
#endif
};
uint16_t Font_22x30_5_Raw[] RODATA= { 
#ifdef PICS_ENABLED_TEST
   #include "font_22x30_5.raw" 
#endif
};
uint16_t Font_22x30_6_Raw[] RODATA= { 
#ifdef PICS_ENABLED_TEST
   #include "font_22x30_6.raw" 
#endif
};
uint16_t Font_22x30_7_Raw[] RODATA= { 
#ifdef PICS_ENABLED_TEST
   #include "font_22x30_7.raw" 
#endif
};
uint16_t Font_22x30_8_Raw[] RODATA= { 
#ifdef PICS_ENABLED_TEST
   #include "font_22x30_8.raw" 
#endif
};
uint16_t Font_22x30_9_Raw[] RODATA= { 
#ifdef PICS_ENABLED_TEST
   #include "font_22x30_9.raw" 
#endif
};
uint16_t Font_22x30_Space_Raw[] RODATA= { 
#ifdef PICS_ENABLED_TEST
   #include "font_22x30_space.raw"
#endif
};

uint16_t * Font_22x30_Data[] RODATA=
{
Font_22x30_Space_Raw , //
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
Font_22x30_Tilde_Raw , //+
NULL, //,
Font_22x30_Cross_Raw ,  //-
NULL, //.
NULL, ///
Font_22x30_0_Raw ,
Font_22x30_1_Raw ,
Font_22x30_2_Raw ,
Font_22x30_3_Raw ,
Font_22x30_4_Raw ,
Font_22x30_5_Raw ,
Font_22x30_6_Raw ,
Font_22x30_7_Raw ,
Font_22x30_8_Raw ,
Font_22x30_9_Raw ,
};

struct Struct_Pic Font_22x30_Pic RODATA={{0,21,0,29} ,0 ,0,0,Rien_Events ,1 ,Font_22x30_Data};
//----------------------------------
