#ifndef DISPLAY_LAYERS
#define DISPLAY_LAYERS

#include "state_machine.h"
//---------------------------------------------------------
#define MAX_PICS	15	//el maximo aparece en workin+pulsante (que son 2) + memorias + memoria grabada. 15
//---------------------------------------------------------
enum Display_Layers_Event_Code
 {
  Info_Modified_Event		=0,
  Structure_Modified_Event	=1,
  All_Updated_Event		=2,
  All_Displayed_Event		=3
 };

//---------------------------------------------------------
extern void 		Init_Display_Layers	(void);
extern State**  	Display_Layers		(void);
extern void 		Display_Layers_Rti	(void);	
//---------------------------------------------------------
struct Struct_Pic_Layer
{
 struct Struct_Pic	*Pic;
 uint8_t 		Actual_Blink;
 uint8_t 		Blink_Period;
 unsigned char 		Blink_State;
};

//---------------------------------------------------------
extern void 		Print_Layers_Used	(void);
//---------------------------------------------------------
extern void 		Do_Blink_Pic		(struct Struct_Pic* Pic,unsigned int Period);
extern void 		Dont_Blink_Pic		(struct Struct_Pic* Pic);
extern void 		Do_Blink_Only_Pic	(struct Struct_Pic* Pic,unsigned int Period);
extern void 		Dont_Blink_Any_Pic	(void);
//---------------------------------------------------------
extern void 		Block_Pic_Events	(struct Struct_Pic* Pic);
extern void 		Unblock_Pic_Events	(struct Struct_Pic* Pic,unsigned char Ecount);
//---------------------------------------------------------
extern void 		Move_Pic2Higth			(unsigned char Bits,	struct Struct_Pic* Pic);
extern void 		Move_Pic2Low			(unsigned char Bits,	struct Struct_Pic* Pic);
extern void 		Move_Pic2Rigth			(unsigned char Bits,	struct Struct_Pic* Pic);
extern void 		Move_Pic2Left			(unsigned char Bits,	struct Struct_Pic* Pic);
extern void 		Add_Pic_On_Top			(struct Struct_Pic* Pic);
extern void 		Add_Pic_On_Bottom		(struct Struct_Pic* Pic);
extern void 		Add_Pic_On_Layer		(struct Struct_Pic* Pic,unsigned char Layer);
extern void 		Del_Pic				(struct Struct_Pic* Pic);
extern void 		Del_All_Layers			(void);
extern unsigned char 	Find_Event_Handler		(unsigned char Button,unsigned char X,unsigned char Y,unsigned char Handler);
extern void 		Layer_Structure_Modified	(void);
extern void 		Layer_Info_Modified		(void);
extern unsigned char 	Layer_Used			(void);
//---------------------------------------------------------
extern void 		Move_Pic_Bits2Higth	(unsigned char Bits,	struct Struct_Pic* Pic);
extern void 		Move_Pic_Bits2Low	(unsigned char Bits,	struct Struct_Pic* Pic);
extern void 		Move_Pic_Bytes2Rigth	(unsigned char Bytes,	struct Struct_Pic* Pic);
extern void 		Move_Pic_Bytes2Left	(unsigned char Bytes,	struct Struct_Pic* Pic);
extern void 		Move_Pic_Bits2Rigth	(unsigned char Bits,	struct Struct_Pic* Pic);
extern void 		Move_Pic_Bits2Left	(unsigned char Bits,	struct Struct_Pic* Pic);
//---------------------------------------------------------
extern void 		All_Displayed		(void);
extern unsigned char 	Disp_Bank_Updated	(void);
//---------------------------------------------------------
	
#endif

