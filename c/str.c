#include <events.h>
#include <str.h>
#include <display_pics.h>

//----------------------------------------------------------------------------------------
void String_Copy_Backward(unsigned char* Source,unsigned char* Destiny,unsigned int Length)
{
 while(Length--) Destiny[Length]=Source[Length];
}
void String_Copy_Forward(unsigned char* Source,unsigned char* Destiny,unsigned int Length)
{
 unsigned int i;
 for(i=0;i<Length;i++) Destiny[i]=Source[i];
}
//----------------------------------------------------------------------------------------
unsigned char String_Cmp(unsigned char* A,unsigned char* B,unsigned char Length)
{
 while(Length-- && A[Length]==B[Length]);
 return !++Length;
}
unsigned char Find_Char_On_String(unsigned char* String,unsigned char Exp,unsigned int Length)
{
 unsigned int i;
 for(i=0;i<Length && String[i]!=Exp;i++);
 return i;
}
unsigned char Long_Compare(unsigned long* A,unsigned long* B)	{return String_Cmp((unsigned char*)A,(unsigned char*)B,4);}
//----------------------------------------------------------------------------------------
void String_Fill(unsigned char* Source,unsigned char Data,unsigned int Length)
{
 while(Length--) Source[Length]=Data;
}
void Slide_Fill(unsigned char* Source,unsigned char Data,unsigned char Data_Length,unsigned char Background,unsigned char Background_Length)
{
 unsigned char Length;
 for(Length=0;Length<Background_Length;Length++) 
	Source[Length]=Length<Data_Length?Data:Background;
}
//----------------------------------------------------------------------------------------
void String2Left(unsigned char* Data,unsigned char Offset,unsigned char Length)
{
 unsigned char i=0;
 for(i=0;i<Length;i++) Data[i]=Data[i+Offset];
}
void Shift_String2Rigth(unsigned char* Source,unsigned int Length, unsigned int Displacement)
{
 while(Length--) Source[Length+Displacement]=Source[Length];
}
//----------------------------------------------------------------------------------------
void Clear_Bit_On_String(unsigned char* Data, unsigned char Bit)
{
 Data[Bit/8]&=~(0x80>>Bit%8);
}
void Set_Bit_On_String(unsigned char* Data, unsigned char Bit)
{
 Data[Bit/8]|=(0x80>>Bit%8);
}
void Clear_Bit_On_String_Array(unsigned char* Data, unsigned char Bit, unsigned char Size, unsigned char Number)
{
 while(Number--) Clear_Bit_On_String(Data+Number*Size,Bit);
}
unsigned char Read_Bit4String(unsigned char* Data,unsigned char Bit)
{
 return (Data[Bit/8]&(0x80>>Bit%8))!=0;
}
//----------------------------------------------------------------------------------------
void Char2Pic(unsigned char Data,struct Struct_Pic *Pic,struct Struct_Pic *Font,unsigned char XPos,unsigned char YPos)
{
	unsigned char X,Y,Font_Offset=0;
	unsigned char **Font_Strings=(unsigned char**)Font->List.Font;
	for(Y=0;Y<Font->EndY;Y++) 
		for(X=0;X<Font->EndX;X++) 
			Pic->List.String[Y+YPos].String[X+XPos]=Font_Strings[Data-' '][Font_Offset++];
}
void String2Pic(unsigned char* String,unsigned char Length,struct Struct_Pic *Pic,struct Struct_Pic *Font,unsigned char XPos,unsigned char YPos)
{
	unsigned char L;
	for(L=0;L<Length;L++) 
		Char2Pic(String[L],Pic,Font,XPos+L*Font->EndX,YPos);
}

