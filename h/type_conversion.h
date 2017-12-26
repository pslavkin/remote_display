#ifndef TYPE_CONVERSION
#define TYPE_CONVERSION

#include "display_phisical.h"
//-----------------------------------------------------
#define Hi(Int)		(*(unsigned char*)&Int)
#define Lo(Int)		(*((unsigned char*)&Int+1))
#define Char(Int)	(*(unsigned char*)&Int+1)
#define Int(Char)	(*(unsigned int*)Char)
#define ABS(a,b) 	(a>b?(a-b):(b-a))
//-----------------------------------------------------
#ifndef   RODATA
	#define RODATA __attribute__ ((section (".const_initialized")))
#endif
#ifndef   FLASH
	#define FLASH __attribute__ ((section ("text")))
#endif
//-----------------------------------------------------
extern unsigned char* 	Long2Bcd 				(unsigned char* Bcd,unsigned long Bin);
extern unsigned char* 	Int2Bcd					(unsigned char* Bcd,unsigned int Bin);
extern unsigned char* 	Int2_3Digit_Bcd				(unsigned char* Bcd,unsigned int Bin);
extern unsigned char*	Int2_4Digit_Bcd				(unsigned char* Bcd,unsigned int Bin);
extern unsigned char* 	Char2Bcd				(unsigned char* Bcd, unsigned char Bin);
extern unsigned char  	Dec_Bcd2Char 				(unsigned char* Bcd);
extern unsigned char* 	Char2_2Digit_Bcd			(unsigned char* Bcd, unsigned char Bin);
extern unsigned int 	Dec_Bcd2Int				(unsigned char* Bcd);
extern unsigned long 	Dec_Bcd2Long 				(unsigned char* Bcd);
extern void 		Wait_Useg				(unsigned int Useg);
//---------------------------------------------------------------------
extern void 		Rigth_Roll		(unsigned char *Data,unsigned char Times,unsigned char Length);
extern void 		Left_Shift		(unsigned char *Data,unsigned char Times,unsigned char Length);
//---------------------------------------------------------------------
extern void 		Rigth_Roll_Jumping	(unsigned char *Data,unsigned char Times,unsigned char Length,unsigned char Jump);
extern void 		Left_Roll_Jumping	(unsigned char *Data,unsigned char Times,unsigned char Length,unsigned char Jump);
//---------------------------------------------------------------------
unsigned char* 		String2Hex_Bcd			(unsigned char* Bcd,unsigned char* String,unsigned char Length);
unsigned char* 		Char2Hex_Bcd			(unsigned char* Bcd,unsigned char Bin);
extern unsigned char* 	Int2Hex_Bcd			(unsigned char* Bcd,unsigned int Bin);
//---------------------------------------------------------------------
extern unsigned char*	Replace_Zero2Space		(unsigned char* Buf,unsigned char Length);
//---------------------------------------------------------------------
extern void Int2Pic		(unsigned int Bin,struct Struct_Pic *Font,struct Struct_Pic *Pic);
extern void Int2_4Digit_Pic	(unsigned int Bin,struct Struct_Pic *Font,struct Struct_Pic *Pic);
extern void Int2_3Digit_Pic	(unsigned int Bin,struct Struct_Pic *Font,struct Struct_Pic *Pic);
extern void Char2Pic		(unsigned char Bin,struct Struct_Pic *Font,struct Struct_Pic *Pic);
extern void Char2_2Digit_Pic	(unsigned char Bin,struct Struct_Pic *Font,struct Struct_Pic *Pic);
extern void Insert_String2Pic	(unsigned char *String,unsigned char Length,struct Struct_Pic *Font,struct Struct_Pic *Pic,unsigned char Offset);
extern void String2Pic		(unsigned char *String,unsigned char Length,struct Struct_Pic *Font,struct Struct_Pic *Pic);
extern void Append_Char2Pic	(unsigned char Char,struct Struct_Pic *Font,struct Struct_Pic *Pic);
//----------------------------------------------------------------

#endif

