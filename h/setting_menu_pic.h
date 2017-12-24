#ifndef SETTING_MENU
#define SETTING_MENU

//------------------------------------------------------
struct Struct_Params
{
 unsigned char	A[2];		//1=100mSeg  	tiempo de rampa asc.    1 a 100
 unsigned char	B[2];		//1=10uSec. 	ancho de pulso 		1 a 40
 unsigned int	C[2];		//1=1hz.  	frecuencia de trabajo 	1 a 3000 canal 1 y 2 
 unsigned char	D[2];		//1=1seg 	tiempo de contraccion.  1 a 120
 unsigned char	E[2];		//1=100mSeg   	tiempo de rampa desc.   1 a 100
 unsigned char	F[2];		//1=1Seg.   	tiempo de descanso 	1 a 120
 unsigned char	G[2];		//1=1repeticion repeticiones del banco  0 a 200 (0=>infinitas)
 unsigned char	H[2];		//0=todos juntos, 1=primero 2 y despues los otros dos
 unsigned char	I[2];		//1=1repeticion repeticiones del efecto de canal 0 a 200 (0=>infititas)
 unsigned char	J[2];		//1=1grado				0 a 180 (0=>en fase, 180 contrafase)
};
//------------------------------------------------------
extern struct Struct_Pic 	Setting_Menu_Icon_Pic;
extern struct Struct_Pic 	Setting_Menu_Label_Pic;
extern void 			Init_Setting_Menu		(void);
extern void 			Set_Bank_Changing		(unsigned char Bank);
extern void 			Set_Bank			(struct Struct_Params* Program);
extern void 			Set_Bank_In_Use			(unsigned char Bank);
extern void 			Toogle_Bank			(void);
extern void 			Add_Setting_Menu		(void); 
extern void 			Del_Setting_Menu		(void);
extern void 			Active_Setting_Menu		(void);
extern void 			Pasive_Setting_Menu		(void);
extern void 			Enable_Setting_Char_Change	(void);
extern void 			Enable_Setting_Int_Change	(void);
extern struct Struct_Params* 	Read_Menu_Params		(void);
extern void 			Update_Bank_Parameters		(void);
//------------------------------------------------------
extern unsigned char 		Read_A				(void);
extern unsigned char 		Read_B				(void);
extern unsigned int 		Read_C				(void);
extern unsigned char 		Read_D				(void);
extern unsigned char 		Read_E				(void);
extern unsigned char 		Read_F				(void);
extern unsigned char 		Read_G				(void);
extern unsigned char 		Read_H				(void);
extern unsigned char 		Read_I				(void);
extern unsigned char 		Read_J				(void);
//------------------------------------------------------
#endif
