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
#include "type_conversion.h"
#include "tpanel.h"






__attribute__((section(".data.FLEX_RAM")))
char flex[100]="\r\nesto esta por defecto en flex\r\n";

__attribute__((section(".testi")))
char sietemil[100]="\r\nesto esta por defecto en flash sietemil\r\n";


void Init_Flash1(void)
{
   Send_NVData2Serial(23,"flash sietemil tiene:\r\n");
   Send_NVData2Serial( 100,sietemil);
}
void Init_Flash2(void)
{
   FTFE->FSTAT=0x30;
   FTFE->FCCOB0=0x81;
   FTFE->FCCOB1=0xFF;
   FTFE->FSTAT|=0x80;
   while((FTFE->FSTAT & 0x80)==0x00)
      ;
   Send_NVData2Serial(13,"flex as ram\r\n");
   Send_Int_NLine2Serial(FTFE->FSTAT);
}
void Init_Flash3(void)
{
   Send_NVData2Serial(14,"esto es flex:\r\n");
   Send_NVData2Serial(100,flex);
}
void Init_Flash4(void)
{
   memcpy(flex,"programo en flash\r\n",19);
   Send_NVData2Serial(13,"cambie flex\r\n");
}
void Init_Flash5(void)
{
   FTFE->FSTAT=0x30;
   FTFE->FCCOB0=0x09;
   FTFE->FCCOB1=0x00;
   FTFE->FCCOB2=0x70;
   FTFE->FCCOB3=0x00;
   FTFE->FSTAT|=0x80;
   while((FTFE->FSTAT & 0x80)==0x00)
      ;
   Send_NVData2Serial(13,"erase block\r\n");
   Send_Int_NLine2Serial(FTFE->FSTAT);
}
void Init_Flash6(void)
{
   FTFE->FSTAT=0x30;
   FTFE->FCCOB0=0x0B;
   FTFE->FCCOB1=0x00;
   FTFE->FCCOB2=0x70;
   FTFE->FCCOB3=0x00;
   FTFE->FCCOB4=0x00;
   FTFE->FCCOB5=0x08;
   FTFE->FSTAT|=0x80;
   while((FTFE->FSTAT & 0x80)==0x00)
      ;
   Send_NVData2Serial(13,"save  block\r\n");
   Send_Int_NLine2Serial(FTFE->FSTAT);
}

void Init_Flash7(void)
{
   LMEM->PCCCR|=0x85000000;
   Send_NVData2Serial(13,"clear cache\r\n");
}
