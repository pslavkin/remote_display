#ifndef TPM
#define TPM

#define FTM2_MOD_VALUE  5000			//20M/5000 = 2khz.. me da justo para que suene el buzzer medianamente bien y bastante decente para filtrar y usar como DAC
#define FTM0_FREQ  	(1) //((BUS_CLK/1)/128) 	//156k
#define FTM1_FREQ  	(1) //(BUS_CLK/1)/128) 	//156k
//------------------------------------------------------------------
extern void 		Init_Buzzer_Ftm_Channels(void);
extern void 		Set_Buzzer_Frec         (unsigned int Value);
extern void 		Init_Ftm2		(void);
//------------------------------------------------------------------
extern void 		Set_Tpm2C0		(unsigned char Pwm);
extern void 		Set_Tpm2C0_Off		(void);
extern void 		Init_Tpm2C0		(void);
//-------
extern void 		Set_Tpm2C1		(unsigned char Pwm);
extern void 		Set_Tpm2C1_Off		(void);
extern void 		Init_Tpm2C1		(void);
//-------
extern void 		Set_Tpm2C2		(unsigned char Pwm);
extern void 		Set_Tpm2C2_Off		(void);
extern void 		Init_Tpm2C2		(void);
//-------
extern void 		Set_Tpm2C3		(unsigned char Pwm);
extern void 		Set_Tpm2C3_Off		(void);
extern void 		Init_Tpm2C3		(void);
//------------------------------------------------------------------
extern void 	Ftm0_Ovf_Irq		(void);
extern void 	Ftm1_CO_Irq		(void);
extern 				void 	Set_Ftm0_Pulse_10uSec 	(unsigned char uSec_x10);
extern 				void 	Set_Ftm0_Period_Hz 	(unsigned int Hz);
extern 				void 	Set_Ftm0_C01_Off	(void);
extern 				void 	Init_Ftm0_C01		(void);			
extern 				void 	Init_Ftm0		(void);
extern 				void 	Show_Feedback		(void);
//------------------------------------------------------------------
extern void 	Ftm1_Ovf_Irq		(void);
extern 				void 	Set_Ftm1_Pulse_10uSec 	(unsigned char uSec_x10);
extern 				void 	Set_Ftm1_Period_Hz 	(unsigned int Hz);
extern 				void 	Set_Ftm1_C01_Off	(void);
extern 				void 	Init_Ftm1_C01		(void);			
extern 				void 	Init_Ftm1		(void);
extern 				void 	Set_Phase_Ftm1		(unsigned char Phase);
//------------------------------------------------------------------
extern unsigned int 		Test_Freq_Pulse_Conflict	(unsigned int Hz);
extern unsigned int 		Test_Pulse_Freq_Conflict	(unsigned int uSec_x10);
//------------------------------------------------------------------
extern void 			Print_Ftm_Values		(void);
#endif
