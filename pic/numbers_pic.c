#include "display_pics.h"
#include "type_conversion.h"
#include "display_layers.h"
#include "stdint.h"
#include "numbers_pic.h"
//------------------------------------------------------
uint16_t Number0_Raw[] RODATA= {/*{{{*/
	#include "number0.raw"
};
uint16_t Number1_Raw[] RODATA= {
	#include "number1.raw"
};
uint16_t Number2_Raw[] RODATA= {
	#include "number2.raw"
};
uint16_t Number3_Raw[] RODATA= {
	#include "number3.raw"
};
uint16_t Number4_Raw[] RODATA= { 
	#include "number4.raw" 
};
uint16_t Number5_Raw[] RODATA= { 
	#include "number5.raw" 
};
uint16_t Number6_Raw[] RODATA= { 
	#include "number6.raw" 
};
uint16_t Number7_Raw[] RODATA= { 
	#include "number7.raw" 
};
uint16_t Number8_Raw[] RODATA= { 
	#include "number8.raw" 
};
uint16_t Number9_Raw[] RODATA= { 
	#include "number9.raw" 
};
uint16_t Number__Raw[] RODATA= { 
	#include "number_.raw" 
};
uint16_t Number_Space_Raw[] RODATA= { 
	#include "number_space.raw" 
};/*}}}*/
uint8_t Actual_Digit=0;
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
