#include "mke18f16.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "everythings.h"
#include "ftm.h"
#include "dma.h"
#include "serial_tx.h"
//-------------------------------------------------------------------
void Init_Ftm3C3(void)
{
	CLOCK_EnableClock(kCLOCK_Ftm3);
	FTM3->CONTROLS[3].CnSC=0x00000065;
	FTM3->MOD=0x001F;
	FTM3->CONTROLS[3].CnV=5;
	FTM3->SC=0x00080008;
}

void Ftm_Print(void)
{
}
void Ftm_Clear(void)
{
}
//-------------------------------------------------------------------------------
