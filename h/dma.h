#ifndef  DMA
#define  DMA

#include "state_machine.h"
#include "display_pics.h"
//-----------------------------------------------------------
enum Dma_Event_Code{
            Dma_Null_Event = 0xD000
                 };
//-----------------------------------------------------------
extern void    Dma_Rti         ( void                                             );
extern void    Init_Dma        ( void                                             );
extern State** Dma             ( void                                             );
extern void    Dma_Request     ( void                                             );
extern void    Print_Destin    ( void                                             );
extern void    Dma_Clear       ( void                                             );
extern void    DMA0_IRQHandler ( void                                             );
extern void    Pic2TCD         ( struct Struct_Pic *Pic,uint8_t Index             );
extern void    Pic2TCD_Mod     ( struct Struct_Pic *Pic,uint8_t Index,uint8_t Mod );
//----------------------------------------------------
#endif

