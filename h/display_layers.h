#ifndef DISPLAY_LAYERS
#define DISPLAY_LAYERS

#include "display_phisical.h"
#include "state_machine.h"
//---------------------------------------------------------
#define MAX_PICS	12	//se probo que oon ?? funca 
//---------------------------------------------------------
enum Display_Layers_Event_Code
 {
  Info_Modified_Event		=0x1A00,
  Structure_Modified_Event	=0x1A01,
  All_Updated_Event		=0x1A02,
  All_Displayed_Event		=0x1A03,
  Display_On_Event		=0x1A04
 };
//---------------------------------------------------------
extern void 		Init_Display_Layers	(void);
extern State**  	Display_Layers		(void);
extern void 		Display_Layers_Rti	(void);	
//---------------------------------------------------------
struct Struct_Pic_Layer
{
 struct Struct_Pic	*Pic;
 unsigned char 		Actual_Blink;		//es el tiempo que se va decrementando...
 unsigned char 		Blink_State;		//bit a bit: 
							//0x01 hide     1->se muestra, 		0->no se muestra
							//0x02 enabled. 1-> procesa scrolls, 	0-> no procesa scrolls
							//0x04 Active.  1->se procesan eventos. 0->no se procesan
 unsigned char 		Actual_HScroll;		//Periodo de scroll horizontal que se va decrementando..
 unsigned char		HScroll[DISP_LINES];	//posicion del scroll horizontal linea x
 unsigned char 		Item_Index;		//posicion que se muestra actualmente
 unsigned char 		Handler_Index;		//posicion que se usa para ejecutar los handlers de la pic, puede que una pic no se mueva pero si se desee que se mueva el parseo de los handlers..
};
//---------------------------------------------------------
extern signed char 	Search_Pic_Pos			(struct Struct_Pic* Pic);
//----------------------------------------------	-----------
extern void 		Do_Blink_Pic			(struct Struct_Pic* Pic);
extern void 		Dont_Blink_Pic			(struct Struct_Pic* Pic);
extern void 		Do_Blink_Only_Pic		(struct Struct_Pic* Pic);
extern void 		Dont_Blink_Any_Pic		(void);
extern void 		Refresh_Pic_Blink		(struct Struct_Pic *Pic);
extern void 		Refresh_Blink			(void);
//----------------------------------------------	-----------
extern void 		Enable_Pic			(struct Struct_Pic* Pic);
extern void 		Disable_Pic			(struct Struct_Pic* Pic);
extern void		Active_Pic			(struct Struct_Pic* Pic);
extern void		Pasive_Pic			(struct Struct_Pic* Pic);
extern void 		Unhide_Pic			(struct Struct_Pic* Pic);
extern void 		Hide_Pic			(struct Struct_Pic* Pic);
extern void		Enable_Periodic_Pic		(struct Struct_Pic* Pic);
extern void		Disable_Periodic_Pic		(struct Struct_Pic* Pic);
//----------------------------------------------	-----------
extern unsigned char 	Does_Pic_Exist			(struct Struct_Pic* Pic);
extern unsigned char 	Is_Pic_On_Top			(struct Struct_Pic* Pic);
//----------------------------------------------	-----------
extern void 		VnScroll2Down_Layer		(unsigned char Layer,unsigned char Times);
extern void 		VnScroll2Down_Pic		(struct Struct_Pic* Pic,unsigned char Times);
extern void 		V1Scroll2Down 			(void);
extern void 		V1Scroll2Down_Pic 		(struct Struct_Pic *Pic); 
extern void 		V2Scroll2Down 			(void);
extern void 		V3Scroll2Down_Pic 		(struct Struct_Pic *Pic);
extern void 		V4Scroll2Down 			(void);
extern void 		V5Scroll2Down 			(void);
extern void 		V6Scroll2Down 			(void);
extern void 		V7Scroll2Down 			(void);
//---------------------------------------------------------
extern void 		VnScroll2Up_Layer		(unsigned char Layer,unsigned char Times);
extern void 		VnScroll2Up_Pic			(struct Struct_Pic* Pic,unsigned char Times);
extern void 		V1Scroll2Up 			(void);
extern void 		V1Scroll2Up_Pic 		(struct Struct_Pic* Pic);
extern void 		V2Scroll2Up 			(void);
extern void 		V3Scroll2Up_Pic 		(struct Struct_Pic* Pic);
extern void 		V4Scroll2Up 			(void);
extern void 		V5Scroll2Up 			(void);
extern void 		V6Scroll2Up 			(void);
extern void 		V7Scroll2Up 			(void);
//---------------------------------------------------------
extern void 		Set_Pic_Item_Index		(struct Struct_Pic* Pic,unsigned char Index);
extern void 		Set_Actual_Pic_Item_Index	(unsigned char Index);
extern unsigned char 	Read_Actual_Pic_Item_Index	(void);
extern unsigned char 	Read_Pic_Item_Index		(struct Struct_Pic* Pic);
extern unsigned char* 	Read_Pic_Item_String_Index	(struct Struct_Pic* Pic,unsigned char Index);
//---------------------------------------------------------
extern void 		Set_Pic_Handler_Index		(struct Struct_Pic* Pic,unsigned char Index);
extern void 		Set_Actual_Pic_Handler_Index	(unsigned char Index);
extern void 		Actual_Pic_Handler_Index2Down	(void);
extern void 		Actual_Pic_Handler_Index2Up	(void);
//---------------------------------------------------------
extern void 		Set_Pic_Item_And_Handler_Index		(struct Struct_Pic* Pic,unsigned char Index);
extern void 		Set_Actual_Pic_Item_And_Handler_Index	(struct Struct_Pic* Pic,unsigned char Index);
//---------------------------------------------------------
extern void 		Move_Pic2Up			(struct Struct_Pic* Pic);
extern void 		Move_Pic2Down			(struct Struct_Pic* Pic);
extern void 		Move_Pic2Top			(struct Struct_Pic* Pic);
extern void 		Move_Pic2Bottom			(struct Struct_Pic* Pic);
extern void 		Move_Layer2Top			(unsigned char Layer);
extern void 		Move_Layer2Bottom		(unsigned char Layer);	
extern void 		Move_Pic2X_Y			(struct Struct_Pic* Pic,unsigned char X,unsigned char Y);
extern void 		Add_Pic_On_Top			(struct Struct_Pic* Pic);
extern void 		Add_Pic_On_Top_Index		(struct Struct_Pic* Pic,unsigned char Index);
extern void 		Add_Pic_On_Bottom		(struct Struct_Pic* Pic);
extern void 		Add_Pic_On_Bottom_Index		(struct Struct_Pic* Pic,unsigned char Index);
extern void 		Add_Pic_On_Layer		(struct Struct_Pic* Pic,unsigned char Layer);
extern void 		Del_Pic				(struct Struct_Pic* Pic);
extern void 		Del_All_Layers			(void);
extern unsigned char 	Find_Event_Handler		(unsigned char Button,unsigned char Handler);
extern void 		Event_Handler_Actual_Event	(void);
extern void 		Event_Handler0_Actual_Event	(void);
extern void 		Event_Handler1_Actual_Event	(void);
extern void 		Event_Handler2_Actual_Event	(void);
extern void 		String_List_Handler0		(void);
extern void 		String_List_Handler1		(void);
extern void 		String_List_Handler2		(void);

extern void 		Pic_String_List_Handler		(struct Struct_Pic* Pic, unsigned char Pos);
extern void 		Pic_String_List_Handler0	(struct Struct_Pic* Pic);
extern void 		Pic_String_List_Handler1	(struct Struct_Pic* Pic);
extern void 		Pic_String_List_Handler2	(struct Struct_Pic* Pic);

extern void 		Layer_Structure_Modified	(void);
extern void 		Layer_Info_Modified		(void);
extern unsigned char 	Layer_Used			(void);
extern unsigned char 	Read_Handler_Layer		(void);
extern void 		Does_Layer_Modified		(void);

//---------------------------------------------------------
extern void 		Print_Layer_Modified		(void);
extern void 		Print_Actual_Layers		(void);
extern void 		Print_Layers_Used		(void);
//---------------------------------------------------------
extern void 		All_Displayed			(void);
extern unsigned char 	Disp_Bank_Updated		(void);
//---------------------------------------------------------
	
#endif

