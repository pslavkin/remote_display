#include "display_pics.h"
#include "type_conversion.h"
#include "display_layers.h"
#include "stdint.h"
#include "numbers_pic.h"
//------------------------------------------------------
uint16_t Number0_Raw[] RODATA= {/*{{{*/
#ifdef PICS_ENABLED
	#include "number0.raw"
#endif
};
uint16_t Number1_Raw[] RODATA= {
#ifdef PICS_ENABLED
	#include "number1.raw"
#endif
};
uint16_t Number2_Raw[] RODATA= {
#ifdef PICS_ENABLED
	#include "number2.raw"
#endif
};
uint16_t Number3_Raw[] RODATA= {
#ifdef PICS_ENABLED
	#include "number3.raw"
#endif
};
uint16_t Number4_Raw[] RODATA= { 
#ifdef PICS_ENABLED
	#include "number4.raw" 
#endif
};
uint16_t Number5_Raw[] RODATA= { 
#ifdef PICS_ENABLED
	#include "number5.raw" 
#endif
};
uint16_t Number6_Raw[] RODATA= { 
#ifdef PICS_ENABLED
	#include "number6.raw" 
#endif
};
uint16_t Number7_Raw[] RODATA= { 
#ifdef PICS_ENABLED
	#include "number7.raw" 
#endif
};
uint16_t Number8_Raw[] RODATA= { 
#ifdef PICS_ENABLED
	#include "number8.raw" 
#endif
};
uint16_t Number9_Raw[] RODATA= { 
#ifdef PICS_ENABLED
	#include "number9.raw" 
#endif
};
uint16_t Number__Raw[] RODATA= { 
#ifdef PICS_ENABLED
	#include "number_.raw" 
#endif
};
uint16_t Number_Space_Raw[] RODATA= { 
#ifdef PICS_ENABLED
	#include "number_space.raw" 
#endif
};/*}}}*/
uint8_t Actual_Digit=0;
uint16_t *Psw[8] RODATA=
{
	Number1_Raw,
	Number2_Raw,
	Number3_Raw,
	Number4_Raw,
	Number5_Raw,
	Number6_Raw,
	Number7_Raw,
	Number8_Raw,
};

uint16_t *Numbers[8]=
{
	Number__Raw,
};
struct Struct_Pic Numbers_Pic=
{
 8,28,257,34,0,0,1,Rien_Events,1,Numbers
};
void Add_Numbers(void) 	{Add_Pic_On_Top(&Numbers_Pic);}
void Del_Numbers(void) 	{Del_Pic(&Numbers_Pic);}

void Reset_Numbers(void)
{
	Actual_Digit=0;
	Numbers_Pic.PCount=1;
	Numbers[0]=Number__Raw;
	Layer_Structure_Modified();
}


void Add_Digit(uint16_t *Digit) 
{
	if(Actual_Digit<8) {
		Numbers[Actual_Digit++]=Digit;
		if(Actual_Digit<8) {
			Numbers_Pic.PCount=Actual_Digit+1;
			Numbers[Actual_Digit]=Number__Raw;
		}
		else Numbers_Pic.PCount=Actual_Digit;
		Layer_Structure_Modified();
	}
}
void Add_Digit0(void) {Add_Digit(Number0_Raw);}
void Add_Digit1(void) {Add_Digit(Number1_Raw);}
void Add_Digit2(void) {Add_Digit(Number2_Raw);}
void Add_Digit3(void) {Add_Digit(Number3_Raw);}
void Add_Digit4(void) {Add_Digit(Number4_Raw);}
void Add_Digit5(void) {Add_Digit(Number5_Raw);}
void Add_Digit6(void) {Add_Digit(Number6_Raw);}
void Add_Digit7(void) {Add_Digit(Number7_Raw);}
void Add_Digit8(void) {Add_Digit(Number8_Raw);}
void Add_Digit9(void) {Add_Digit(Number9_Raw);}

void Del_Digit(void)  
{
	if(Actual_Digit>0) {
		Actual_Digit--;
		Numbers_Pic.PCount=Actual_Digit+1; 
		Numbers[Actual_Digit]=Number__Raw;
		Layer_Structure_Modified();
	}
}
unsigned char Psw_Compare(void)
{
	uint8_t i=0;
	if(Actual_Digit==8) 
		for(;i<8 && Psw[i]==Numbers[i];i++) 
			;
	return i==8;
} 



