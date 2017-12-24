#include "lptmr.h"
#include "mke18f16.h"
#include "fsl_clock.h"
//---------------------------------------------------
void Init_Lptmr(void)
{
    	CLOCK_EnableClock(kCLOCK_Lptmr0);
    	CLOCK_SetIpSrc(kCLOCK_Lptmr0, kCLOCK_IpSrcSysPllAsync);
	
	LPTMR0->PSR=0x00000000; //60Mhz/2
	LPTMR0->CMR=60000; //30M/60k=0.5k

	LPTMR0->CSR =0x000000C0;
	LPTMR0->CSR|=0x00000001;
	NVIC_EnableIRQ(LPTMR0_IRQn);
}

void LPTMR0_IRQHandler 	(void)		
{
	LPTMR0->CSR|=0x00000080;
}
//---------------------------------------------------

