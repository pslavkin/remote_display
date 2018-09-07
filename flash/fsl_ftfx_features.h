#ifndef _FSL_FTFX_FEATURES_H_
#define _FSL_FTFX_FEATURES_H_

#include "fsl_common.h"
#include "fsl_ftfx_adapter.h"

// FTFx configuration
/*Flash driver location. */
#if !defined(FTFx_DRIVER_IS_FLASH_RESIDENT)
#if (!defined(BL_TARGET_ROM) && !defined(BL_TARGET_RAM))
#define FTFx_DRIVER_IS_FLASH_RESIDENT 1 /*!< Used for the flash resident application. */
#else
#define FTFx_DRIVER_IS_FLASH_RESIDENT 0 /*!< Used for the non-flash resident application. */
#endif
#endif

/*! @brief Flash Driver Export option */
#if !defined(FTFx_DRIVER_IS_EXPORTED)
#if defined(BL_TARGET_ROM)
#define FTFx_DRIVER_IS_EXPORTED 1 /*!< Used for the ROM bootloader. */
#else
#define FTFx_DRIVER_IS_EXPORTED 0 /*!< Used for the MCUXpresso SDK application. */
#endif
#endif
/*@}*/

#define FTFx_DRIVER_HAS_FLASH1_SUPPORT (0)
#define FTFx_FLASH_COUNT (1)

// @name Secondary flash configuration
/*! @brief Indicates whether the secondary flash has its own protection register in flash module. */
#define FTFx_FLASH1_HAS_PROT_CONTROL (0)

/*! @brief Indicates whether the secondary flash has its own Execute-Only access register in flash module. */
#define FTFx_FLASH1_HAS_XACC_CONTROL (0)

#define FTFx_FLASH1_IS_INDEPENDENT_BLOCK (0)
#endif /* _FSL_FTFX_FEATURES_H_ */

