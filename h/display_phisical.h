#ifndef DISPLAY_PHISICAL
#define DISPLAY_PHISICAL

#include <stdint.h>

struct Struct_Pic_Events
{
 uint16_t Upper_X;	
 uint16_t Upper_Y;
 uint16_t Lower_X;
 uint16_t Lower_Y;
 unsigned char Button;
 void (* Handler[3]) (void);
};

struct Struct_Pic
{
 uint16_t Start_X;
 uint16_t End_X;
 uint16_t Start_Y;
 uint16_t End_Y;
 uint8_t Blink_Period;
 unsigned char Alpha;
 unsigned char ECount;				//numero de eventos
 struct Struct_Pic_Events* Events; 		//
 unsigned char PCount;				//Pic count, numero de pics.... usualmente 1, pero mas de uno para strings.
 uint16_t **Data;				//
};

//---------------------------------------------------
extern void 		Init_Display_Phisical		(void);
//---------------------------------------------------
extern unsigned char**	Read_Disp_Bank			(void);
extern void 		Clear_Bank			(void);
extern void 		Wait_Disp_Ready 		(void);
extern void 		Send_Disp_Bank2Serial		(void);
extern void 		Init_Lcd_Pins			(void);
//---------------------------------------------------
extern void 		Write_Next			(void);
extern void 		Write_Custom_Char		(unsigned char Pos,unsigned char* Pic);
extern void 		Write_Custom_Bank		(unsigned char Length,unsigned char* Bank); 
extern void 		Clear_Lcd			(void);
extern void 		Pic2Lcd				(struct Struct_Pic *Pic);
extern void 		Lcd2Pic				(struct Struct_Pic *Pic);
//---------------------------------------------------
extern void 		Disp_CS_Set			(void);
extern void 		Disp_CS_Clr			(void);
extern void 		Disp_DI_Set			(void);
extern void 		Disp_DI_Clr			(void);
extern void 		Disp_Rst_Set			(void);
extern void 		Disp_Rst_Clr			(void);
extern void 		Disp_WR_Set			(void);
extern void 		Disp_WR_Clr			(void);
extern void 		Disp_RD_Set			(void);
extern void 		Disp_RD_Clr			(void);
extern unsigned char 	Disp_Data_Read			(void);
#endif

