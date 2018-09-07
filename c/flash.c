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
#include "log_pic.h"
#include "flash.h"

__attribute__((section(".uflash1")))
struct Log_Table FLog={
   {  "11111111+",
      "22222222+",
      "33333333-",
      "44444444+",
      "55555555-",
   }
};
void Print_FLog(void)
{
   Send_NLine_NVData2Serial(sizeof(FLog),FLog.Line[0]);
}

void Init_Flash(void)
{
   FlexRam_As_Ram();
   if(FLog.Line[0][0]!=0xFF)
      *Read_RLog()=FLog;
   else
      memcpy(Read_RLog()->Line[0], "11111111+\0""22222222+\0""33333333-\0" "44444444+\0" "55555555-\0",sizeof(FLog));
}

void Save_Table_Log(void)
{
   FErase((void*)&FLog);
   Save_FlexRam2Flash((void*)&FLog,sizeof(FLog));
   Invalidate_Cache();
}

void FErase(void* Address)
{
   uint32_t A=(uint32_t)Address;
   A&=0x00FFFC00;
   FTFE->FSTAT=0x30;
   FTFE->FCCOB0=0x09;
   FTFE->FCCOB1=(A>>16);
   FTFE->FCCOB2=(A>> 8);
   FTFE->FCCOB3=(A>> 0);
   FTFE->FSTAT|=0x80;
   while((FTFE->FSTAT & 0x80)==0x00)
      ;
}
void FlexRam_As_Ram(void)
{
   FTFE->FSTAT=0x30;
   FTFE->FCCOB0=0x81;
   FTFE->FCCOB1=0xFF;
   FTFE->FSTAT|=0x80;
   while((FTFE->FSTAT & 0x80)==0x00)
      ;
}
void Save_FlexRam2Flash(void* Address,uint32_t Length)
{
   uint32_t A=(uint32_t)Address;
   A&=0x00FFFC00;
   Length=Length/16+Length%16;             //cada grabacion son 16 bytes
   FTFE->FSTAT   = 0x30;
   FTFE->FCCOB0  = 0x0B;
   FTFE->FCCOB1  = (A>>16);
   FTFE->FCCOB2  = (A>> 8);
   FTFE->FCCOB3  = (A>> 0);
   FTFE->FCCOB4  = Length>>8;
   FTFE->FCCOB5  = Length>>0;
   FTFE->FSTAT  |= 0x80;
   while((FTFE->FSTAT & 0x80)==0x00)
      ;
}
void Invalidate_Cache(void)
{
   LMEM->PCCCR|=0x85000000;
}
