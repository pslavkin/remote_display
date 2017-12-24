#ifndef DISPLAY_PICS
#define DISPLAY_PICS

#include <state_machine.h>

struct Struct_String_List
{
 unsigned char 	Scroll;
 unsigned char* String;
 void (* Handler[3]) (void);	//un handler para cada boton
};
struct Struct_Pic_Events
{
 unsigned char Button;		//codigo del boton..
 void (* Handler[3]) (void);	//uno para press otro para hold y otro para released
};
struct Struct_Pic
{
 unsigned char StartX;
 unsigned char StartY;
 unsigned char EndX;
 unsigned char EndY;
 unsigned char Blink_Period;
 unsigned char HScroll_Period;
 unsigned char ECount;				//numero de eventos
 struct Struct_Pic_Events* Events; 		//
 union {
	struct Struct_String_List* String;
	char** Font;
 } List;
};
extern struct Struct_Pic_Events Rien_Events[];
//---------------
extern void 				Init_Display_Pics			(void);
//---------------
extern void 				Add_Welcome				(void);
extern void 				Del_Welcome				(void);
//---------------
extern void 				Add_Time				(void);
extern void 				Del_Time				(void);
//---------------
extern void 				Init_Font_5x4				(void);
extern void 				Init_Font_2x2				(void);
extern void 				Init_Font_3x3				(void);
//---------------

extern void				Add_Lock				(void);
extern void				Del_Lock				(void);
extern void 				Enable_Lock				(void);
extern void 				Disable_Lock				(void);
extern unsigned int 			Read_Lock_Pass 				(void);
extern unsigned int 			Unlocked 				(void);
extern void 	 			Lock 					(void); 
extern void 				Unlock 					(void); 
extern void				Active_Lock				(void);
extern void				Pasive_Lock				(void);
//---------------
extern void 				Add_Settings_Menu			(void);
extern void 				Del_Settings_Menu			(void);
//---------------
extern void 				Add_User_Menu				(void);
extern void 				Del_User_Menu				(void);
//---------------
extern void 				Add_Off					(void);
extern void 				Del_Off					(void);
//---------------
extern void 				Add_Setting_Param			(void);
extern void 				Del_Setting_Param			(void);
//---------------
#endif

