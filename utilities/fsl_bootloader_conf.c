#include <stdint.h>

/*!
* @brief Defines the structure to set the Bootloader Configuration Area
*
* This type of variable is used to set the Bootloader Configuration Area
* of the chip.
*
* Warning: some configuration may not work on the devices that donot support 
* corresponding feature.
*/
typedef struct BootloaderConfiguration
{
    uint32_t tag; //!< [00:03] Magic number to verify bootloader configuration is
                  //! valid. Must be set to 'kcfg'.
    uint32_t crcStartAddress; //!< [04:07] Start address for application image CRC
                              //! check. If the bits are all set then Kinetis
                              //! bootloader by default will not perform any CRC 
                              //! check.
    uint32_t crcByteCount; //!< [08:0b] Byte count for application image CRC
                           //! check. If the bits are all set then Kinetis
                           //! bootloader by default will not prform any CRC check.
    uint32_t crcExpectedValue; //!< [0c:0f] Expected CRC value for application CRC
                               //! check. If the bits are all set then Kinetis
                               //! bootloader by default will not perform any CRC
                               //! check.
    uint8_t enabledPeripherals; //!< [10:10] Bitfield of peripherals to enable.
                                //! bit 0 - LPUART, bit 1 - I2C, bit 2 - SPI,
                                //! bit 3 - CAN, bit 4 - USB
                                //! Kinetis bootloader will enable the peripheral if
                                //! corresponding bit is set to 1.
    uint8_t i2cSlaveAddress; //!< [11:11] If not 0xFF, used as the 7-bit I2C slave
                             //! address. If 0xFF, defaults to 0x10
                             //! for I2C slave address.
    uint16_t peripheralDetectionTimeoutMs; //!< [12:13] Timeout in milliseconds
                                           //! for active peripheral detection. If
                                           //! 0xFFFF, defaults to 5 seconds.
    uint16_t usbVid; //!< [14:15] Sets the USB Vendor ID reported by the device
                     //! during enumeration. If 0xFFFF, it defaults to 0x15A2.
    uint16_t usbPid; //!< [16:17] Sets the USB Product ID reported by the device
                     //! during enumeration.
    uint32_t usbStringsPointer; //!< [18:1b] Sets the USB Strings reported by the
                                //! device during enumeration.
    uint8_t clockFlags; //!< [1c:1c] The flags in the clockFlags configuration
                        //! field are enabled if the corresponding bit is cleared (0).
                        //! bit 0 - HighSpeed Enable high speed mode (i.e., 48 MHz).
    uint8_t clockDivider; //!< [1d:1d] Inverted value of the divider to use for
                          //! core and bus clocks when in high speed mode.
    uint8_t bootFlags; //!< [1e:1e] If bit 0 is cleared, then Kinetis bootloader
                       //! will jump to either Quad SPI Flash or internal flash
                       //! image depending on FOPT BOOTSRC_SEL bits.
                       //! If the bit is set, then Kinetis bootloader will prepare
                       //! for host communication over serial peripherals.
    uint8_t pad0; //!< [1f:1f] Reserved, set to 0xFF
    uint32_t mmcauConfigPointer; //!< [20:23] A pointer to the MMCAU configuration
                                 //! structure in memory.
    uint32_t keyBlobPointer; //!< [24:27]Holds a pointer value to the key blob
                             //! array used to configure OTFAD
    uint8_t pad1; //!< [28:28] reserved
    uint8_t canConfig1; //!< [29:29] ClkSel[1], PropSeg[3], SpeedIndex[4]
    uint16_t canConfig2; //!< [2a:2b] Pdiv[8], Pseg1[3], Pseg2[3],  rjw[2]
    uint16_t canTxId; //!< [2c:2d] txId
    uint16_t canRxId; //!< [2e:2f] rxId
    uint32_t qspi_config_block_pointer; //!< [30:33] A pointer to the QSPI config
                                        //! block in internal flash array.
    uint32_t reserved[3]; //!<[34:3f] reserved
} bootloader_config_t;

/* Bootlader configuration area */
__attribute__((section(".BootloaderConfig"))) const bootloader_config_t BootloaderConfig =
    {
        .tag 				= 0x6766636B, 	//!< Magic Number
        .crcStartAddress 		= 0xFFFFFFFF, 	//!< Disable CRC check
        .crcByteCount 			= 0xFFFFFFFF, 	//!< Disable CRC check
        .crcExpectedValue 		= 0xFFFFFFFF, 	//!< Disable CRC check
        .enabledPeripherals 		= 0xE1, 	//!< Enabled Peripheral: UART 
        .i2cSlaveAddress 		= 0xFF, 	//!< Use default I2C address(0x10)
        .peripheralDetectionTimeoutMs 	= 0x03E8, 	//!< Use user-defined timeout(ms)
//        .peripheralDetectionTimeoutMs 	= 0x0064, 	//!< Use user-defined timeout(ms)
        .usbVid 			= 0xFFFF, 	//!< Use default Vendor ID(0x15A2)
        .usbPid 			= 0xFFFF, 	//!< Use default Product ID(0x0073)
        .usbStringsPointer 		= 0xFFFFFFFF, 	//!< Use default USB String
        .clockFlags 			= 0xFF, 	//!< Disable High speed mode
        .clockDivider 			= 0xFF, 	//!< Use clock divider(0)
        .bootFlags 			= 0xFF, 	//!< Enable communication with host
        .mmcauConfigPointer 		= 0xFFFFFFFF, 	//!< No MMCAU configuration
        .keyBlobPointer 		= 0xFFFFFFFF, 	//!< No key blob
        .canConfig1 			= 0xFF, 	//!< Use default canConfig1
        .canConfig2 			= 0xFFFF, 	//!< Use default canConfig2
        .canTxId 			= 0xFFFF, 	//!< Use default CAN TX ID(0x123)
        .canRxId 			= 0xFFFF, 	//!< Use default CAN RX ID(0x321)
        .qspi_config_block_pointer 	= 0xFFFFFFFF, 	//!< No QSPI configuration 
    };
