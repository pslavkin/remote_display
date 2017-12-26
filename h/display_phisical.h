#ifndef DISPLAY_PHISICAL
#define DISPLAY_PHISICAL

#include <stdint.h>

#define DISP_LINES	4
#define DISP_COLUMNS	20
//-----PORT D map--------------------------------------------
//   - - - E  R D - -
//   - - - -  / / - -
//   - - - -  W I - -
//0b 0 0 0 0  0 0 0 0

#define CLEAR_CODE		0x01
#define RETURN_CODE		0x03
#define SHIFT_MODE_CODE		0x06
#define DISP_ON_CODE 		0x0C
#define DISP_OFF_CODE 		0x08
#define CURSOR_ON_CODE 		0x0E
#define CURSOR_OFF_CODE 	0x0C
#define FUN_SET_CODE 		0x38
#define SET_DDRAM_CODE		0x80
#define SET_CGRAM_CODE		0x40
//-------------- WINSTAR ------------------
#define INSTR_DELAY 		50
#define DATA_DELAY 		50
//---------------------------------------------------
extern void 		Init_Display_Phisical		(void);
extern uint8_t	Disp_Bank[DISP_LINES][DISP_COLUMNS];
//---------------------------------------------------
extern unsigned char**	Read_Disp_Bank			(void);
extern void 		Clear_Bank			(void);
extern void 		Wait_Disp_Ready 		(void);
extern void 		Write_Disp_Bank			(void);
extern void 		Send_Disp_Bank2Serial		(void);
extern void 		Init_Lcd_Pins			(void);
//---------------------------------------------------
extern void 		Write_Next			(void);
extern void 		Write_Custom_Char		(unsigned char Pos,unsigned char* Pic);
extern void 		Write_Custom_Bank		(unsigned char Length,unsigned char* Bank); 
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

static inline void Disp_Data_Write	(unsigned char Data)	
{
	GPIOA->PDOR=(GPIOA->PDOR&0xFFFF00FF)|(Data<<8);
}
extern unsigned char 	Disp_Data_Read			(void);
#endif

