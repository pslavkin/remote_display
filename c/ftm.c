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
//                    |------30----------|
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
   FTM3->CONTROLS[0].CnSC = 0x00000069; // habilita irq pero no para que salte sino para que funcione DMA. tiene que estar los dos en 1.
   FTM3->MOD              = 30;         // ahora anda con 30!! con 20 detona, pero 30 va de lujo!!! 
                                        //son 120M/30 son 4Megas x 16bits en cada strobe son 64mbits de transferencia bruto!!!! tremendo!! con SPI a lo sumo 20..
   FTM3->CONTROLS[0].CnV  = FTM3->MOD-5;
   FTM3->SC               = 0x00010008;
}
//--------------------------------------------------------------------------------
uint16_t  Wished_Ftm0Ch[1]; // aca escribo lo que quiero en el tpm que llegue lentamente

void Init_Ftm0C0(void) //bligth
{
   CLOCK_EnableClock ( kCLOCK_Ftm0  );
   CLOCK_EnableClock ( kCLOCK_PortC );         // lo usa bligth por ahora...
   FTM0->CONTROLS[0].CnSC = 0x00000028;        // pwm sin irq baja en match
   FTM0->MOD              = 60000;             //
   FTM0->CONTROLS[0].CnV  = 0;                 // arranca apagado
   FTM0->SC               = 0x00010009;        // habilito el pwm0 y seto clk 120M/2
   PORT_SetPinMux ( PORTC, 0, kPORT_MuxAlt2 ); // PTC0 como alt2 es FTM0CH0
   Set_Bligth(80);
}

void Inc10_Bligth ( void ) {
   uint8_t P=((uint32_t)Wished_Ftm0Ch[0]*100)/FTM0->MOD;
   if(P<=90) Set_Bligth(P+10);
}
void Dec10_Bligth ( void ) {
   uint8_t P=((uint32_t)Wished_Ftm0Ch[0]*100)/FTM0->MOD;
   if(P>=10) Set_Bligth(P-10);
}
void Inc_Bligth ( void ) {
   uint8_t P=((uint32_t)Wished_Ftm0Ch[0]*100)/FTM0->MOD;
   if(P<100) Set_Bligth(P+1);
}
void Dec_Bligth ( void ) {
   uint8_t P=((uint32_t)Wished_Ftm0Ch[0]*100)/FTM0->MOD;
   if(P>  0) Set_Bligth(P-1);
}
void Set_Bligth(uint8_t Percent) {
   Set2Accelerate_Ftm0(0,(FTM0->MOD*Percent)/100);
   Send_NVData2Serial(11,"Set Bligth:");
   Send_Int_NLine2Serial(Percent);
}
//--------------------------------------------------------------------------------
void     Set2Accelerate_Ftm0 ( uint8_t Ch ,uint16_t Pwm) {
   Wished_Ftm0Ch[Ch] = Pwm;
}
void     Accelerate_Ftm0_Log ( uint8_t Ch ) {
   FTM0->CONTROLS[Ch].CnV= (Wished_Ftm0Ch[Ch]+FTM0->CONTROLS[Ch].CnV)/2;
}
void     Accelerate_All_Ftm0 ( void ) {
   Accelerate_Ftm0_Log ( 0 );
}
void Print_BLigth(void) {
   Send_Int_NLine2Serial(Wished_Ftm0Ch[0]);
}
//-------------------------------------------------------------------------------
