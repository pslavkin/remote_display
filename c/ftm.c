#include "mke18f16.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "everythings.h"
#include "ftm.h"
#include "dma.h"
#include "serial_tx.h"
//-------------------------------------------------------------------
//    WR     ____     ______________     ______________     ____________
//              |     |            |     |            |     |
//              |_____|            |_____|            |_____|
//
//                                 |--12-|
//                    |------40----------|
//                                       <------| en este instante se latchea el dato. justo antes de que DMA ponga el nuevo
//DMA                              |
//                                 |
//Data                              <------| esto e slo que tarde entre que salta IRQ/DMA de ftm hasta que el DMA pone el dato nuevo
//          _______________________________
//                                         |
//                                         |___________
void Init_Ftm3C0(void)
{
   CLOCK_EnableClock(kCLOCK_Ftm3);
   FTM3->CONTROLS[0].CnSC = 0x00000069;      //habilita irq pero no para que salte sino para que funcione DMA. tiene que estar los dos en 1.
   FTM3->MOD              = 60;  //con 80 anda bien.. aunque por osci se ve que podria andar con 30... el tema es que podria cada tanto demorarse un poquito mas y como es open loop, se puede chiflar un bit
   FTM3->CONTROLS[0].CnV  = FTM3->MOD-5;
   FTM3->SC               = 0x00010008;
}

void Ftm_Print(void)
{
}
void Ftm_Clear(void)
{
}
//-------------------------------------------------------------------------------
