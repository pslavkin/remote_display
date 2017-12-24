#ifndef STRING
#define STRING

#include <display_pics.h>

#define String_Copy(Source,Destiny,Length)	String_Copy_Backward(Source,Destiny,Length)
extern void 		String_Copy_Forward		(unsigned char* Source,unsigned char* Destiny,unsigned int Length);
extern unsigned char 	Find_Char_On_String		(unsigned char* String,unsigned char Exp,unsigned int Length);
extern void 		String_Copy_Backward		(unsigned char* Source,unsigned char* Destiny,unsigned int Length);
extern unsigned char 	String_Cmp			(unsigned char* A,unsigned char* B,unsigned char Length);
extern unsigned char 	Long_Compare			(unsigned long* A,unsigned long*  B);
extern void 		String_Fill			(unsigned char* Source,unsigned char Data,unsigned int Length);
extern void 		Slide_Fill			(unsigned char* Source,unsigned char Data,unsigned char Data_Length,unsigned char Background,unsigned char Background_Length);
extern void 		String2Left			(unsigned char* Data,unsigned char Offset,unsigned char Length);
extern void 		Shift_String2Rigth		(unsigned char* Source,unsigned int Length, unsigned int Displacement);
extern void 		Clear_Bit_On_String		(unsigned char* Data, unsigned char Bit);
extern void 		Set_Bit_On_String		(unsigned char* Data, unsigned char Bit);
extern void 		Clear_Bit_On_String_Array	(unsigned char* Data, unsigned char Bit, unsigned char Size, unsigned char Number);
extern unsigned char 	Read_Bit4String			(unsigned char* Data,unsigned char Bit);
extern void 		Char2Pic			(unsigned char Data,struct Struct_Pic *Pic,struct Struct_Pic *Font,unsigned char XPos,unsigned char YPos);
extern void 		String2Pic			(unsigned char* String,unsigned char Length,struct Struct_Pic *Pic,struct Struct_Pic *Font,unsigned char XPos,unsigned char YPos);

#endif
