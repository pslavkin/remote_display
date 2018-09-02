#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "mke18f16.h"
#include "fsl_lpuart.h"
#include "fsl_port.h"
#include "everythings.h"
#include "state_machine.h"
#include "events.h"
#include "serial_tx.h"
#include "schedule.h"
#include "leds_session.h"
#include "display_phisical.h"
#include "display_layers.h"
#include "type_conversion.h"
#include "ftm.h"
#include "dma.h"
#include "tpanel.h"
//-------------------------------------------------------------

void Init_Adc(void)
{
   CLOCK_EnableClock ( kCLOCK_Adc1);
   CLOCK_SetIpSrc    ( kCLOCK_Adc1, kCLOCK_IpSrcSysPllAsync ); // habilito clk al periferico y le mando 60Mhz
   ADC1->CFG1=4;  //clk sin division y 12 bits
}

uint16_t Read_Adc(unsigned char Channel)
{
    ADC1->SC1[0]=Channel;
    while((ADC1->SC1[0]&0x00000080)==0)
       ;  //espera a que la conversion termine puleando el bt CCF del ATDSC1...que deberia tardar 0.5uSeg que no es nada no vale la pena usar irqs.....
    return ADC1->R[0];
}
void           Print_Adc12 ( void )
{
   Send_Int_NLine2Serial(Read_Adc(12));
}
//-------------------------------------------------------------

