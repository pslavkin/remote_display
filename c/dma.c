#include "mke18f16.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "everythings.h"
#include "state_machine.h"
#include "events.h"
#include "schedule.h"
#include "dma.h"
#include "type_conversion.h"
#include "str.h"
#include "serial_tx.h"

static  State
	Off[];

State *Dma_Sm;

unsigned char Source[9]="hola dma";
unsigned char Destin[9]="";

//-------------------------------------------------------------------
void Init_Dma(void)
{
	Dma_Sm=Off;
	CLOCK_EnableClock(kCLOCK_Dmamux0);
	CLOCK_EnableClock(kCLOCK_Dma0);
	DMAMUX->CHCFG[0]=60;		//configura el source del canal cero como la fuente cero que es siempre enable

	DMA0->TCD[0].SADDR=(uint32_t)Source;
	DMA0->TCD[0].SOFF=1;
	DMA0->TCD[0].ATTR=0;
	DMA0->TCD[0].NBYTES_MLNO=2;
	DMA0->TCD[0].SLAST=0;
	DMA0->TCD[0].DADDR=(uint32_t)Destin;
	DMA0->TCD[0].DOFF=1;
	DMA0->TCD[0].CITER_ELINKNO=4;
	DMA0->TCD[0].BITER_ELINKNO=4;
	DMA0->TCD[0].DLAST_SGA=0;

	DMAMUX->CHCFG[0]|=0x80;		//con este bit prendo 
	//DMA0->ERQ|=0x0001;		//este es el que activa el request
	//New_Periodic_Func_Schedule(10,Dma_Request);
}
void Dma_Clear(void)
{
	String_Fill(Destin,'-',sizeof(Destin));
}
void Dma_Request(void)
{
	//DMA0->SSRT=0;		//activa el canal cero, es equivalente a DMA0->TCD[0].CSR|=1;
	DMA0->ERQ|=0x0001;		//este es el que activa el request
}

void Print_Destin(void)
{
	Send_NVData2Serial(sizeof(Destin),Destin);
}
State** 	Dma		(void)		{return &Dma_Sm;}
void 		Dma_Rti		(void)		{}
//-------------------------------------------------------------------
static State Off[] RODATA =
{
 {ANY_Event		,Rien		,Off},
};
//-------------------------------------------------------------------------------
