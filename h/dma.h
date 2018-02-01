#ifndef  DMA
#define  DMA

#include "state_machine.h"
//-----------------------------------------------------------
enum Dma_Event_Code{
				Dma_Null_Event	= 0xD000
		           };
//-----------------------------------------------------------
extern void 	Dma_Rti	(void);
extern void 	Init_Dma	(void);
extern State** 	Dma		(void);
extern void 	Dma_Request	(void);
extern void 	Print_Destin	(void);
//----------------------------------------------------
#endif

