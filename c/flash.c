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
#include <stdint.h>

__attribute__((section(".uflash")))
struct Log_Table FLog={
   {  "         ",
      "         ",
      "         ",
      "         ",
      "         ",
   }
};
void Init_Flash(void)
{
   FlexRam_As_Ram();             //en teoria arranca la flex como ram normal, pero por si acaso. Ojo que cuando hago esto se resetea toda la ram a 0xFF
   if(FLog.Line[0][0]!=0xFF)     //que no se haya chiflado la flash
      *Read_RLog()=FLog;         //copio estructuras asi como asi
   else                          //si fallo la flash resetea la lista (sino arrancaria con todo 0xFF
      memcpy(Read_RLog()->Line[0],
            "        +\0""        -\0""        +\0""        -\0""        +\0",
            sizeof(FLog));
}
//void Atomic_Save_Table_Log ( void ) { Atomic(Save_Table_Log());}
void Save_Table_Log        ( void )
{
Atomic(
   FErase             ( (void* )&FLog)             ;
   Save_FlexRam2Flash ( (void* )&FLog,sizeof(FLog));
   Invalidate_Cache   (        )                   ;
   );
}
uint8_t Flash_Command(uint8_t Cmd, void* Address, uint16_t Length)
{
   uint32_t A=(uint32_t)Address&0x00FFE000; // los sectores son de 0x1000 byte.. vuelo el resto de los bits
   Length=Length/16+Length%16;              // cada grabacion son 16 bytes, si piden 17, grabo 32
   FTFE->FSTAT   = 0x30;                    // borro cualquier error previo
   FTFE->FCCOB0  = Cmd;                     // comando
   FTFE->FCCOB1  = (A>>16);                 // direccion
   FTFE->FCCOB2  = (A>> 8);
   FTFE->FCCOB3  = (A>> 0);
   FTFE->FCCOB4  = Length>>8;               // largo de datos (puede que no se requiera)
   FTFE->FCCOB5  = Length>>0;
   FTFE->FSTAT  |= 0x80;                    // arranca el comando
   while((FTFE->FSTAT & 0x80)==0x00)        // esta garantizado que termina o termina...
      ;
   return FTFE->FSTAT;                      // aca vuelve algo de informacion de errorers basicos
}
void FErase ( void* Address ) {
   Flash_Command ( FLASH_SECTOR_ERASE_CMD ,Address,0 );
}
void FlexRam_As_Ram ( void ) {
   Flash_Command ( FLASH_FLEX_AS_RAM_CMD ,(void*)0xFFFFFFFF ,0 );
}
void Save_FlexRam2Flash ( void* Address,uint32_t Length ) {
   Flash_Command ( FLASH_SECTOR_SAVE_CMD ,Address ,Length );
}
void Invalidate_Cache ( void ) {
   LMEM->PCCCR|=0x85000000;
}
void Print_FLog(void) {
   Send_NLine_NVData2Serial(sizeof(FLog),FLog.Line[0]);
}
