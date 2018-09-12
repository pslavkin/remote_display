#ifndef TYPE_CONVERSION
#define TYPE_CONVERSION

#include "display_phisical.h"
//-----------------------------------------------------
#define Hi(Int)      (*(unsigned char*)&Int)
#define Lo(Int)      (*((unsigned char*)&Int+1))
#define Char(Int) (*(unsigned char*)&Int+1)
#define Int(Char) (*(unsigned int*)Char)
#define ABS(a,b)  (a>b?(a-b):(b-a))
//-----------------------------------------------------
#ifndef   RODATA
   #define RODATA __attribute__ ((section (".const_initialized")))
#endif
#ifndef   FLASH_SECTION
   #define FLASH_SECTION __attribute__ ((section ("text")))
#endif
//-----------------------------------------------------
extern char*            Long2Bcd         ( char* Bcd ,unsigned long Bin  );
extern char*            Int2Bcd          ( char* Bcd ,unsigned int Bin   );
extern char*            Int2_3Digit_Bcd  ( char* Bcd ,unsigned int Bin   );
extern char*            Int2_4Digit_Bcd  ( char* Bcd ,unsigned int Bin   );
extern char*            Char2Bcd         ( char* Bcd ,unsigned char Bin );
extern unsigned char    Dec_Bcd2Char     ( char* Bcd                    );
extern char*            Char2_2Digit_Bcd ( char* Bcd ,unsigned char Bin );
extern unsigned int     Dec_Bcd2Int      ( char* Bcd                    );
extern unsigned long    Dec_Bcd2Long     ( char* Bcd                    );
extern void             Wait_Useg        ( unsigned int Useg            );
//---------------------------------------------------------------------
extern void       Rigth_Roll     (unsigned char *Data,unsigned char Times,unsigned char Length);
extern void       Left_Shift     (unsigned char *Data,unsigned char Times,unsigned char Length);
//---------------------------------------------------------------------
extern void       Rigth_Roll_Jumping   (unsigned char *Data,unsigned char Times,unsigned char Length,unsigned char Jump);
extern void       Left_Roll_Jumping (unsigned char *Data,unsigned char Times,unsigned char Length,unsigned char Jump);
//---------------------------------------------------------------------
extern char*       String2Hex_Bcd       (char* Bcd,char* String,unsigned char Length);
extern char*       Char2Hex_Bcd         (char* Bcd,unsigned char Bin);
extern char*   Int2Hex_Bcd       (char* Bcd,uint16_t  Bin);
extern char* String2Char_Array(char* Char_Array,char* String,unsigned char Length);
//-----------------Bcd----------------------------------------------------
extern char*   Replace_Zero2Space      (char* Buf,unsigned char Length);
//---------------------------------------------------------------------


#endif

