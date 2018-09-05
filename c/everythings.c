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
#include "adc.h"
#include "tpanel.h"

State
   Free_State1 [ ],
   Free_State2 [ ],
   Free_State3 [ ],
   Free_State4 [ ],
   Free_State5 [ ],
   Free_State6 [ ],
   Free_State7 [ ],
   Free_State8 [ ],
   Free_State9 [ ],
   Free_State10[ ];

State* Everythings_Sm;           //variable que lleva cuenta del estado de la maquina de estados de "detodo un poco"...
//-------------------------------------------------------------------
void Init_Ack_Pin(void)
{
   CLOCK_EnableClock  ( kCLOCK_PortE               );
   PORT_SetPinMux     ( PORTE ,14 ,kPORT_MuxAsGpio );
   PORT_Pullup_Enable ( PORTE ,14                  );
   GPIO_PinInit_As_In ( GPIOE ,14                  );
}
bool Read_Ack_Pin(void)
{
   return  GPIO_PinRead(GPIOE,14);
}
//----------------------------------------------------------------------------------------------------
void     Init_Everythings  (void)
{
   Everythings_Sm=Free_State1;
   Init_Schedule              ( );
   Init_Leds_Session          ( );
   Init_Display_Phisical_7789 ( );
   Init_Display_Layers        ( );
   Init_Dma                   ( );
   Init_Ftm3C0                ( );
   Init_Adc                   ( );
   Init_Tpanel                ( );
   Init_Ack_Pin               ( );
}  
State**  Everythings     ( void ) { return &Everythings_Sm             ;} // devuelve la direccion de la maquina de estados Everythings para poder mandarle mensajes.
void     Everythings_Rti ( void ) { Send_Event(ANY_Event,Everythings());} // manda mensajes ANY a tiempos predefinidos...


#include "fsl_ftfx_controller.h"
/*! @brief Flash driver Structure */
static flexnvm_config_t s_flashDriver;
void error_trap   ( void ) { Send_NVData2Serial ( 8,"\r\nHALTED" )                      ;}
void app_finalize ( void ) { Send_NVData2Serial(34,"End of FlexNVM EEprom Example \r\n");}


void Init_Flash(void)
{
    ftfx_security_state_t securityStatus = kFTFx_SecurityStateNotSecure; /* Return protection status */
    status_t result; /* Return code from each flash driver function */

    /* Clean up Flash driver Structure*/
    memset(&s_flashDriver, 0, sizeof(flexnvm_config_t));

    /* print welcome message */
    Send_NVData2Serial("\r\n FlexNVM EEprom Example Start \r\n");

    /* Setup flash driver structure for device and initialize variables. */
    result = FLEXNVM_Init(&s_flashDriver);
    if (kStatus_FTFx_Success != result) error_trap();

    /* Check security status. */
    result = FLEXNVM_GetSecurityState(&s_flashDriver, &securityStatus);
    if (kStatus_FTFx_Success != result) error_trap();

    /* Print security status. */
    switch (securityStatus)
    {
        case kFTFx_SecurityStateNotSecure:
            Send_NVData2Serial(9,"\r\nUNSECURE");
            break;
        case kFTFx_SecurityStateBackdoorEnabled:
            Send_NVData2Serial(25,"\r\nSECURE BACKDOOR ENABLED");
            break;
        case kFTFx_SecurityStateBackdoorDisabled:
            Send_NVData2Serial(26,"\r\nSECURE BACKDOOR DISABLED");
            break;
        default:
            break;
    }
    Send_NVData2Serial("\r\n");

    /* Debug message for user. */
    /* Test flexnvm dflash feature only if flash is unsecure. */
    if (kFTFx_SecurityStateNotSecure != securityStatus)
    {
        Send_NVData2Serial(58,"\r\nEEprom opeation will not be executed, as Flash is SECURE");
        app_finalize();
    }
    else
    {
        uint32_t s_buffer[BUFFER_LEN];     /* Buffer for program */
        uint32_t s_buffer_rbc[BUFFER_LEN]; /* Buffer for readback */
        uint32_t destAdrss;                /* Address of the target location */

        uint32_t dflashBlockBase = 0;
        uint32_t dflashTotalSize = 0;
        uint32_t eepromBlockBase = 0;
        uint32_t eepromTotalSize = 0;
        uint32_t flexramBlockBase = 0;
        uint32_t flexramTotalSize = 0;

        /* Get flash properties*/
        FLEXNVM_GetProperty(&s_flashDriver ,kFLEXNVM_PropertyDflashBlockBaseAddr  ,&dflashBlockBase);
        FLEXNVM_GetProperty(&s_flashDriver ,kFLEXNVM_PropertyFlexRamBlockBaseAddr ,&flexramBlockBase);
        FLEXNVM_GetProperty(&s_flashDriver ,kFLEXNVM_PropertyFlexRamTotalSize     ,&flexramTotalSize);
        FLEXNVM_GetProperty(&s_flashDriver ,kFLEXNVM_PropertyEepromTotalSize      ,&eepromTotalSize);

        if (!eepromTotalSize)
        {
            /* Note: The EEPROM backup size must be at least 16 times the EEPROM partition size in FlexRAM. */
            uint32_t eepromDataSizeCode   = EEPROM_DATA_SET_SIZE_CODE;
            uint32_t flexnvmPartitionCode = FLEXNVM_PARTITION_CODE ;

            Send_NVData2Serial("\r\n There is no available EEprom (FlexNVM) on this Device by default.");
            Send_NVData2Serial("\r\n Example is trying to configure FlexNVM block as EEprom.");

            result = FLEXNVM_ProgramPartition(&s_flashDriver   ,kFTFx_PartitionFlexramLoadOptLoadedWithValidEepromData ,
                                            eepromDataSizeCode ,flexnvmPartitionCode);
            if (kStatus_FTFx_Success != result)
            {
                error_trap();
            }

            /* Reset MCU */
            Send_NVData2Serial("\r\n Perform a system reset \r\n");
            NVIC_SystemReset();
        }

        FLEXNVM_GetProperty(&s_flashDriver, kFLEXNVM_PropertyDflashTotalSize, &dflashTotalSize);
        eepromBlockBase = dflashBlockBase + dflashTotalSize;

        /* Print flash information - EEprom. */
        Send_NVData2Serial(23,"\r\n EEprom Information: ");
        Send_NVData2Serial("\r\n EEprom Base Address: (0x%x) ", eepromBlockBase);
        Send_NVData2Serial("\r\n EEprom Total Size:\t%d B", eepromTotalSize);

        Send_NVData2Serial("\r\n Make FlexRAM available for EEPROM ");
        result = FLEXNVM_SetFlexramFunction(&s_flashDriver, kFTFx_FlexramFuncOptAvailableForEeprom);
        if (kStatus_FTFx_Success != result)
        {
            error_trap();
        }
        Send_NVData2Serial("\r\n Now EEPROM data is read and written by accessing the FlexRAM address space ");
        Send_NVData2Serial("\r\n FlexRAM Base Address: (0x%x) ", flexramBlockBase);

        /* Print message for user. */
        /* Prepare buffer. */
        for (uint32_t i = 0; i < BUFFER_LEN; i++)
        {
            s_buffer[i] = i + 1;
        }

        Send_NVData2Serial("\r\n Read 16 bytes data from start of EEPROM space");
        destAdrss = flexramBlockBase;
        for (uint32_t i = 0; i < BUFFER_LEN; i++)
        {
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
            /* Clean the D-Cache before reading the flash data*/
            SCB_CleanInvalidateDCache();
#endif
            s_buffer_rbc[i] = *(volatile uint32_t *)(destAdrss + i * 4);
            if (s_buffer_rbc[i] == s_buffer[i])
            {
                if (i == BUFFER_LEN - 1)
                {
                    Send_NVData2Serial("\r\n The first 16 bytes data in EEPROM are what we have progrommed before");

                    for (uint32_t i = 0; i < BUFFER_LEN; i++)
                    {
                        s_buffer[i] = 0xFFFFFFFFU;
                    }
                    result = FLEXNVM_EepromWrite(&s_flashDriver, destAdrss, (uint8_t *)s_buffer, sizeof(s_buffer));
                    if (kStatus_FTFx_Success != result)
                    {
                        error_trap();
                    }
                    Send_NVData2Serial("\r\n Recover the first 16 bytes memory space of EEprom as 0xFFs");
                    app_finalize();
                }
            }
            else if (s_buffer_rbc[i] != 0)
            {
                Send_NVData2Serial("\r\n The first 16 bytes data in EEPROM are not all 0x00s");
                Send_NVData2Serial("\r\n Program the first 16 bytes memory space of EEprom as 0x00s");
                /* Prepare buffer. */
                for (uint32_t i = 0; i < BUFFER_LEN; i++)
                {
                    s_buffer[i] = 0;
                }
                result = FLEXNVM_EepromWrite(&s_flashDriver, destAdrss, (uint8_t *)s_buffer, sizeof(s_buffer));
                if (kStatus_FTFx_Success != result)
                {
                    error_trap();
                }
                Send_NVData2Serial("\r\n Successfully Programmed Location 0x%x -> 0x%x \r\n", destAdrss,
                       (destAdrss + sizeof(s_buffer)));

                /* Reset MCU */
                Send_NVData2Serial("\r\n Perform a system reset \r\n");
                NVIC_SystemReset();
            }
            else
            {
                if (i == BUFFER_LEN - 1)
                {
                    Send_NVData2Serial("\r\n The first 16 bytes data in EEPROM are all 0x00s");
                }
            }
        }

        Send_NVData2Serial("\r\n Program a buffer(16 bytes) into the first 16 bytes memory space of EEprom");
        result = FLEXNVM_EepromWrite(&s_flashDriver, destAdrss, (uint8_t *)s_buffer, sizeof(s_buffer));
        if (kStatus_FTFx_Success != result)
        {
            error_trap();
        }
        Send_NVData2Serial("\r\n Successfully Programmed Location 0x%x -> 0x%x \r\n", destAdrss, (destAdrss + sizeof(s_buffer)));

        /* Reset MCU */
        Send_NVData2Serial("\r\n Perform a system reset \r\n");
        NVIC_SystemReset();
    }

    app_finalize();

    return 0;
}

}












//----------------------------------------------------------------------------------------------------
State Free_State1 [ ] RODATA  =
{
{ANY_Event ,Rien             ,Free_State2} ,
};
State Free_State2 [ ] RODATA  =
{{
 ANY_Event ,Rien             ,Free_State3  ,
}};
State Free_State3 [ ] RODATA  =
{{
 ANY_Event ,Rien             ,Free_State4  ,
}};
State Free_State4 [ ] RODATA  =
{{
 ANY_Event ,Rien             ,Free_State5  ,
}};
State Free_State5 [ ] RODATA  =
{{
 ANY_Event ,Rien             ,Free_State6  ,
}};
State Free_State6 [ ] RODATA  =
{{
 ANY_Event ,Schedule         ,Free_State7  ,
}};
State Free_State7 [ ] RODATA  =
{{
 ANY_Event ,Rien             ,Free_State8  ,
}};
State Free_State8 [ ] RODATA  =
{{
 ANY_Event ,Led_Effects_Func ,Free_State9  ,
}};
State Free_State9 [ ] RODATA  =
{{
 ANY_Event ,Rien             ,Free_State10 ,
}};
State Free_State10[ ] RODATA  =
{{
 ANY_Event ,Rien             ,Free_State1  ,
}};
//-------------------------------------------------------------------------------
