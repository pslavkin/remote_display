#include "serial_session.h"
#include "serial_rx.h"
#include "serial_tx.h"
#include "state_machine.h"
#include "events.h"
#include "serial_phisical.h"
#include "everythings.h"
#include "rti.h"
#include "type_conversion.h"
#include "display_layers.h"
#include "welcome_pic.h"
#include "mask_pic.h"
#include "accept_reject_pic.h"
#include "dma.h"
#include "ftm.h"
#include "adc.h"
#include "tpanel.h"

State
   Parsing_Main[],
   Parsing_Working[],
   Parsing_Layers[],
   Parsing_Ftm[],
   Parsing_Dma[],
   Parsing_Adc[],
   Parsing_Tpanel[];

State* Serial_Session_Sm;
//---------------------------------------------------------------------
unsigned char About_Menu[] RODATA=
{
 "NOTO N1104 DCIV5.4\r\n"
};
unsigned char Main_Menu[] RODATA=
{
 "NOTO N1104 Main menu\r\n"
 "A Working\r\n"
 "B Buzzer\r\n"
 "C Info\r\n"
 "D Layers\r\n"
 "E Dma\r\n"
 "F Ftm\r\n"
 "G Adc\r\n"
 "H Tpanel\r\n"
 "R Reset\r\n"
 ". About\r\n"
 "? Help\r\n"
};
unsigned char Working_Menu[] RODATA=
{
 "Working menu\r\n"
 "< Back\r\n"
 "? Help\r\n"
};
unsigned char Layers_Menu[] RODATA=
{
 "Layers menu\r\n"
 "A Actual Layer\r\n"
 "B Layer Modified\r\n"
 "C Layer Used\r\n"
 "0 Mask0\r\n"
 "1 Mask1\r\n"
 "2 Mask2\r\n"
 "3 Mask3\r\n"
 "4 Mask4\r\n"
 "5 Mask5\r\n"
 "6 Mask6\r\n"
 "7 Mask7\r\n"
 "8 Mask8\r\n"
 "9 Mask9\r\n"
 "D Mask Del\r\n"
 "E Mask Enter\r\n"
 "F Reboot\r\n"
 "G Init Dump\r\n"
 "H Dump\r\n"
 "I End Dump\r\n"
 "J Red\r\n"
 "K Green\r\n"
 "L Blue\r\n"
 "< Back\r\n"
 "? Help\r\n"
};
unsigned char Dma_Menu[] RODATA=
{
 "Dma menu\r\n"
 "A Print Destin\r\n"
 "B Request DMA\r\n"
 "C Clear DMA\r\n"
 "< Back\r\n"
 "? Help\r\n"
};
unsigned char Ftm_Menu[] RODATA=
{
 "Ftm menu\r\n"
 "A Clear OVF\r\n"
 "B Print OVF\r\n"
 "< Back\r\n"
 "? Help\r\n"
};
unsigned char Adc_Menu[] RODATA=
{
 "Adc menu\r\n"
 "A Read Channel 12\r\n"
 "< Back\r\n"
 "? Help\r\n"
};
unsigned char Tpanel_Menu[] RODATA=
{
 "Tpanel menu\r\n"
 "A Print Raw\r\n"
 "< Back\r\n"
 "? Help\r\n"
};
//---------------------------------------------------------------------
void Print_About_Menu   ( void ) { Send_NVData2Serial(sizeof(About_Menu)-1   ,(unsigned char*)About_Menu)    ;}
void Print_Main_Menu    ( void ) { Send_NVData2Serial(sizeof(Main_Menu)-1    ,(unsigned char*)Main_Menu)     ;}
void Print_Layers_Menu  ( void ) { Send_NVData2Serial(sizeof(Layers_Menu)-1  ,(unsigned char*)Layers_Menu)   ;}
void Print_Working_Menu ( void ) { Send_NVData2Serial(sizeof(Working_Menu)-1 ,(unsigned char*)Working_Menu)  ;}
void Print_Ftm_Menu     ( void ) { Send_NVData2Serial(sizeof(Ftm_Menu)-1     ,(unsigned char*)Ftm_Menu)      ;}
void Print_Dma_Menu     ( void ) { Send_NVData2Serial(sizeof(Dma_Menu)-1     ,(unsigned char*)Dma_Menu)      ;}
void Print_Adc_Menu     ( void ) { Send_NVData2Serial(sizeof(Adc_Menu)-1     ,(unsigned char*)Adc_Menu)      ;}
void Print_Tpanel_Menu  ( void ) { Send_NVData2Serial(sizeof(Tpanel_Menu)-1     ,(unsigned char*)Tpanel_Menu);}
//--------------------------------------------------------------------
State**  Serial_Session    (void)   {return &Serial_Session_Sm;} 
void     Init_Serial_Session  (void)
{
 Serial_Session_Sm = Parsing_Main;
 Init_Serial_Rx();
 Init_Serial_Tx();
}
//---------------------------------------------------------------------
State Parsing_Main   [ ]RODATA=
{
{ 'A'       ,Rien                      ,Parsing_Working },
{ 'D'       ,Print_Layers_Menu         ,Parsing_Layers  },
{ 'E'       ,Print_Dma_Menu            ,Parsing_Dma     },
{ 'F'       ,Print_Ftm_Menu            ,Parsing_Ftm     },
{ 'G'       ,Print_Adc_Menu            ,Parsing_Adc     },
{ 'H'       ,Print_Tpanel_Menu         ,Parsing_Tpanel  },
{ '.'       ,Print_About_Menu          ,Parsing_Main    },
{ '?'       ,Print_Main_Menu           ,Parsing_Main    },
{ ANY_Event ,Rien                      ,Parsing_Main    },
                                                        };
State Parsing_Working[ ]RODATA=
{
{ '<'       ,Rien                      ,Parsing_Main    },
{ '?'       ,Print_Working_Menu        ,Parsing_Working },
{ ANY_Event ,Rien                      ,Parsing_Working },
                                                        };
State Parsing_Layers [ ]RODATA=
{
// {'A'     ,Print_Actual_Layers       ,Parsing_Layers}  ,
// {'B'     ,Print_Layer_Modified      ,Parsing_Layers}  ,
// {'C'     ,Print_Layers_Used         ,Parsing_Layers}  ,
{ '0'       ,Add_Mask0                 ,Parsing_Layers  },
{ '1'       ,Add_Mask1                 ,Parsing_Layers  },
{ '2'       ,Add_Mask2                 ,Parsing_Layers  },
{ '3'       ,Add_Mask3                 ,Parsing_Layers  },
{ '4'       ,Add_Mask4                 ,Parsing_Layers  },
{ '5'       ,Add_Mask5                 ,Parsing_Layers  },
{ '6'       ,Add_Mask6                 ,Parsing_Layers  },
{ '7'       ,Add_Mask7                 ,Parsing_Layers  },
{ '8'       ,Add_Mask8                 ,Parsing_Layers  },
{ '9'       ,Add_Mask9                 ,Parsing_Layers  },
{ 'D'       ,Add_Mask_Delete           ,Parsing_Layers  },
{ 'E'       ,Add_Mask_Enter            ,Parsing_Layers  },
{ 'F'       ,Clear_All_And_Add_Welcome ,Parsing_Layers  },
{ 'G'       ,Init_Dump_Lcd             ,Parsing_Layers  },
{ 'H'       ,Dump_Lcd                  ,Parsing_Layers  },
{ 'I'       ,End_Dump_Lcd              ,Parsing_Layers  },
{ 'J'       ,Red_Lcd                   ,Parsing_Layers  },
{ 'K'       ,Green_Lcd                 ,Parsing_Layers  },
{ 'L'       ,Blue_Lcd                  ,Parsing_Layers  },
{ '<'       ,Rien                      ,Parsing_Main    },
{ '?'       ,Print_Layers_Menu         ,Parsing_Layers  },
{ ANY_Event ,Rien                      ,Parsing_Layers  },
                                                        };
State Parsing_Dma    [ ]RODATA=
{
{ 'A'       ,Print_Destin              ,Parsing_Dma     },
{ 'B'       ,Dma_Request               ,Parsing_Dma     },
{ 'C'       ,Dma_Clear                 ,Parsing_Dma     },
{ '<'       ,Rien                      ,Parsing_Main    },
{ '?'       ,Print_Dma_Menu            ,Parsing_Dma     },
{ ANY_Event ,Rien                      ,Parsing_Dma     },
                                                        };
State Parsing_Ftm    [ ]RODATA=
{
{ 'A'       ,Ftm_Clear                 ,Parsing_Ftm     },
{ 'B'       ,Ftm_Print                 ,Parsing_Ftm     },
{ '<'       ,Rien                      ,Parsing_Main    },
{ '?'       ,Print_Ftm_Menu            ,Parsing_Ftm     },
{ ANY_Event ,Rien                      ,Parsing_Ftm     },
                                                        };
State Parsing_Adc    [ ]RODATA=
{
{ 'A'       ,Print_Adc12               ,Parsing_Adc     },
{ '<'       ,Rien                      ,Parsing_Main    },
{ '?'       ,Print_Adc_Menu            ,Parsing_Adc     },
{ ANY_Event ,Rien                      ,Parsing_Adc     },
                                                        };
State Parsing_Tpanel [ ]RODATA=
{
{ 'A'       ,Print_TPanel_Raw          ,Parsing_Tpanel  },
{ '<'       ,Rien                      ,Parsing_Main    },
{ '?'       ,Print_Tpanel_Menu         ,Parsing_Tpanel  },
{ ANY_Event ,Rien                      ,Parsing_Tpanel  },
                                                        };
//------------------------------------------------------------------------------
