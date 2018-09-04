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
uint16_t Number0_Raw[] RODATA= {/*{{{*/
#ifdef PICS_ENABLED
   #include "number0.raw"
#endif
};
uint16_t Number1_Raw[] RODATA= {
#ifdef PICS_ENABLED
   #include "number1.raw"
#endif
};
uint16_t Number2_Raw[] RODATA= {
#ifdef PICS_ENABLED
   #include "number2.raw"
#endif
};
uint16_t Number3_Raw[] RODATA= {
#ifdef PICS_ENABLED
   #include "number3.raw"
#endif
};
uint16_t Number4_Raw[] RODATA= { 
#ifdef PICS_ENABLED
   #include "number4.raw" 
#endif
};
uint16_t Number5_Raw[] RODATA= { 
#ifdef PICS_ENABLED
   #include "number5.raw" 
#endif
};
uint16_t Number6_Raw[] RODATA= { 
#ifdef PICS_ENABLED
   #include "number6.raw" 
#endif
};
uint16_t Number7_Raw[] RODATA= { 
#ifdef PICS_ENABLED
   #include "number7.raw" 
#endif
};
uint16_t Number8_Raw[] RODATA= { 
#ifdef PICS_ENABLED
   #include "number8.raw" 
#endif
};
uint16_t Number9_Raw[] RODATA= { 
#ifdef PICS_ENABLED
   #include "number9.raw" 
#endif
};
uint16_t Number__Raw[] RODATA= { 
#ifdef PICS_ENABLED
   #include "number_.raw" 
#endif
};
uint16_t Number_Space_Raw[] RODATA= { 
#ifdef PICS_ENABLED
   #include "number_space.raw" 
#endif
};/*}}}*/

uint16_t * Font_30x42_Data[] RODATA=
{
Number_Space_Raw , // 
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
Number__Raw , //-
NULL, //.
NULL, ///
Number0_Raw ,
Number1_Raw ,
Number2_Raw ,
Number3_Raw ,
Number4_Raw ,
Number5_Raw ,
Number6_Raw ,
Number7_Raw ,
Number8_Raw ,
Number9_Raw ,
};

struct Struct_Pic Font_30x42_Pic RODATA={{0,29,0,41} ,0 ,0,0,Rien_Events ,1 ,Font_30x42_Data};
//----------------------------------
