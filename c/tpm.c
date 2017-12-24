#include <tpm.h>
#include <serial_tx.h>
#include <everythings.h>
#include <buzzer.h> //debug
#include <display_phisical.h> //debug
#include <schedule.h> //debug
#include <type_conversion.h> //debug

unsigned int Temp_Mod0;
unsigned int Temp_Mod1;
unsigned int Temp_Ftm0_C0V;
unsigned int Temp_Ftm1_C0V;
static unsigned char Feedback;
//------------------------------------------------------------------------------------------------
void Init_Buzzer_Ftm_Channels(void)
{
}
void    Set_Buzzer_Frec         (unsigned int Value)
{
}
//-----------TPM2C0---AMPLITUD CANAL 2--------------------------
void Set_Tpm2C0			(unsigned char Pwm)	{} //FTM2_MOD_VALUE+1-Pwm*(FTM2_MOD_VALUE/100);}
void Set_Tpm2C0_Off		(void)			{} //FTM2_MOD_VALUE+1;}
void Init_Tpm2C0		(void)			
{
}
//-----------TPM2C1---AMPLITUD CANAL 1--------------------------
void Set_Tpm2C1			(unsigned char Pwm)	{} //FTM2_MOD_VALUE+1-Pwm*(FTM2_MOD_VALUE/100);}
void Set_Tpm2C1_Off		(void)			{} //FTM2_MOD_VALUE+1;}
void Init_Tpm2C1		(void)			
{
}
//-----------TPM2C2---AMPLITUD CANAL 3--------------------------
void Set_Tpm2C2			(unsigned char Pwm)	{} //FTM2_MOD_VALUE+1-Pwm*(FTM2_MOD_VALUE/100);}
void Set_Tpm2C2_Off		(void)			{} //FTM2_MOD_VALUE+1;}
void Init_Tpm2C2		(void)			
{
}
//-----------TPM2C3---AMPLITUD CANAL 4--------------------------
void Set_Tpm2C3			(unsigned char Pwm)	{} //FTM2_MOD_VALUE+1-Pwm*(FTM2_MOD_VALUE/100);}
void Set_Tpm2C3_Off		(void)			{} //FTM2_MOD_VALUE+1;}
void Init_Tpm2C3		(void)			
{
}
//----------------------------------------------------------------------------------------------------
void Init_Ftm2			(void)
{
	Init_Buzzer_Ftm_Channels();
	Set_Buzzer_Frec(0);
	Init_Tpm2C0();
	Init_Tpm2C1();
	Init_Tpm2C2();
	Init_Tpm2C3();
}
//-----------FTM0-----------------------------------------------------------------------------------------
 void Ftm0_Ovf_Irq(void)
{
}
void Show_Feedback		(void)
{
	Send_Char_NLine2Serial(Feedback);
}
void Init_Ftm0			(void)
{
}
//-----------Ftm0_C01-----------------------------
void Set_Ftm0_Pulse_10uSec	(unsigned char uSec_x10)
{
}
void Set_Ftm0_Period_Hz 	(unsigned int Hz)
{
}
void Set_Ftm0_C01_Off		(void)			
{
}
void Init_Ftm0_C01		(void)			
{
}
//-----------Ftm1_C01-----------------------------
void Set_Ftm1_Pulse_10uSec 	(unsigned char uSec_x10)
{
}
void Set_Ftm1_Period_Hz 	(unsigned int Hz)
{
}
void Set_Ftm1_C01_Off		(void)		
{
}
void Init_Ftm1_C01		(void)			
{
}
//-----------TEST---------------------------------
unsigned int Test_Pulse_Freq_Conflict(unsigned int uSec_x10)
{
 return 1;
}
unsigned int Test_Freq_Pulse_Conflict(unsigned int Hz)
{
 return 1;
}
//-----------FTM1-----------------------------------------------------------------------------------------
void Ftm1_Ovf_Irq(void)
{
}
void Init_Ftm1			(void)
{
}
//-----------PHASE-----------------------------------------------------------------------------------------
void Ftm1_CO_Irq(void)
{
}
void Set_Phase_Ftm1		(unsigned char Phase)
{
}
//----------------------------------------------------------------------------------------------------
void Print_Ftm_Values(void)
{
	unsigned char Buf[50];
	Send_VData2Serial(50,Buf);
	Send_NLine2Serial();
}
 
