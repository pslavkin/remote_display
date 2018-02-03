#include "mke18f16.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "everythings.h"
#include "ftm.h"
#include "dma.h"
#include "serial_tx.h"
#include "display_phisical.h"
#include "welcome_pic.h"

unsigned char OVF=0;
//-------------------------------------------------------------------
void Init_Ftm3C3(void)
{
	CLOCK_EnableClock(kCLOCK_Ftm3);
//	FTM3->CONTROLS[3].CnSC=0x00000028;
	FTM3->CONTROLS[3].CnSC=0x00000065;
	FTM3->CONTROLS[3].CnV=0xFFFF/3;
	FTM3->SC=0x00080008;
	NVIC_EnableIRQ(FTM3_IRQn);
}

void Ftm_Print(void)
{
	Send_Char_NLine2Serial(OVF);
}
void Ftm_Clear(void)
{
	Set_Frame_Address(&Data_Pic);
	Write_Disp_Instr(0x2C);	
	DMA0->ERQ|=0x0001;		//este es el que activa el request
	OVF=0;
    	PORT_SetPinMux		(PORTC, 7, kPORT_MuxAlt4);	//write active low
}
void FTM3_IRQHandler(void)
{
	FTM3->SC&=~0x00000200;
//	FTM3->CONTROLS[3].CnSC&=~0x00000080;
	OVF=1;
}




//-------------------------------------------------------------------------------
