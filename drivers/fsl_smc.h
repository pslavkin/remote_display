#ifndef _FSL_SMC_H_
#define _FSL_SMC_H_

#include "fsl_common.h"

/*! @addtogroup smc */
/*! @{ */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief SMC driver version 2.0.3. */
#define FSL_SMC_DRIVER_VERSION (MAKE_VERSION(2, 0, 3))
/*@}*/

/*!
 * @brief Power Modes Protection
 */
typedef enum _smc_power_mode_protection
{
    kSMC_AllowPowerModeVlp = SMC_PMPROT_AVLP_MASK, /*!< Allow Very-Low-power Mode.        */
    kSMC_AllowPowerModeHsrun = SMC_PMPROT_AHSRUN_MASK, /*!< Allow High-speed Run mode.        */
    kSMC_AllowPowerModeAll = (0U
                              |
                              SMC_PMPROT_AVLP_MASK
                              |
                              kSMC_AllowPowerModeHsrun
                              ) /*!< Allow all power mode.              */
} smc_power_mode_protection_t;

/*!
 * @brief Power Modes in PMSTAT
 */
typedef enum _smc_power_state
{
    kSMC_PowerStateRun = 0x01U << 0U,  /*!< 0000_0001 - Current power mode is RUN   */
    kSMC_PowerStateStop = 0x01U << 1U, /*!< 0000_0010 - Current power mode is STOP  */
    kSMC_PowerStateVlpr = 0x01U << 2U, /*!< 0000_0100 - Current power mode is VLPR  */
    kSMC_PowerStateVlpw = 0x01U << 3U, /*!< 0000_1000 - Current power mode is VLPW  */
    kSMC_PowerStateVlps = 0x01U << 4U, /*!< 0001_0000 - Current power mode is VLPS  */
    kSMC_PowerStateHsrun = 0x01U << 7U /*!< 1000_0000 - Current power mode is HSRUN */
} smc_power_state_t;

/*!
 * @brief Run mode definition
 */
typedef enum _smc_run_mode
{
    kSMC_RunNormal = 0U, /*!< Normal RUN mode.             */
    kSMC_RunVlpr = 2U,   /*!< Very-low-power RUN mode.     */
    kSMC_Hsrun = 3U /*!< High-speed Run mode (HSRUN). */
} smc_run_mode_t;

/*!
 * @brief Stop mode definition
 */
typedef enum _smc_stop_mode
{
    kSMC_StopNormal = 0U, /*!< Normal STOP mode.           */
    kSMC_StopVlps = 2U,   /*!< Very-low-power STOP mode.   */
} smc_stop_mode_t;


/*!
 * @brief Partial STOP option
 */
typedef enum _smc_partial_stop_mode
{
    kSMC_PartialStop = 0U,  /*!< STOP - Normal Stop mode*/
    kSMC_PartialStop1 = 1U, /*!< Partial Stop with both system and bus clocks disabled*/
    kSMC_PartialStop2 = 2U, /*!< Partial Stop with system clock disabled and bus clock enabled*/
} smc_partial_stop_option_t;

/*!
 * @brief SMC configuration status.
 */
enum _smc_status
{
    kStatus_SMC_StopAbort = MAKE_STATUS(kStatusGroup_POWER, 0) /*!< Entering Stop mode is abort*/
};

/*!
 * @brief IP version ID definition.
 */
typedef struct _smc_version_id
{
    uint16_t feature; /*!< Feature Specification Number. */
    uint8_t minor;    /*!< Minor version number.         */
    uint8_t major;    /*!< Major version number.         */
} smc_version_id_t;

/*!
 * @brief IP parameter definition.
 */
typedef struct _smc_param
{
    bool hsrunEnable; /*!< HSRUN mode enable. */
    bool llsEnable;   /*!< LLS mode enable.   */
    bool lls2Enable;  /*!< LLS2 mode enable.  */
    bool vlls0Enable; /*!< VLLS0 mode enable. */
} smc_param_t;



/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*! @name System mode controller APIs*/
/*@{*/

/*!
 * @brief Gets the SMC version ID.
 *
 * This function gets the SMC version ID, including major version number,
 * minor version number, and feature specification number.
 *
 * @param base SMC peripheral base address.
 * @param versionId     Pointer to the version ID structure.
 */
static inline void SMC_GetVersionId(SMC_Type *base, smc_version_id_t *versionId)
{
    *((uint32_t *)versionId) = base->VERID;
}

/*!
 * @brief Gets the SMC parameter.
 *
 * This function gets the SMC parameter including the enabled power mdoes.
 *
 * @param base SMC peripheral base address.
 * @param param         Pointer to the SMC param structure.
 */
void SMC_GetParam(SMC_Type *base, smc_param_t *param);

/*!
 * @brief Configures all power mode protection settings.
 *
 * This function  configures the power mode protection settings for
 * supported power modes in the specified chip family. The available power modes
 * are defined in the smc_power_mode_protection_t. This should be done at an early
 * system level initialization stage. See the reference manual for details.
 * This register can only write once after the power reset.
 *
 * The allowed modes are passed as bit map. For example, to allow LLS and VLLS,
 * use SMC_SetPowerModeProtection(kSMC_AllowPowerModeVlls | kSMC_AllowPowerModeVlps).
 * To allow all modes, use SMC_SetPowerModeProtection(kSMC_AllowPowerModeAll).
 *
 * @param base SMC peripheral base address.
 * @param allowedModes Bitmap of the allowed power modes.
 */
static inline void SMC_SetPowerModeProtection(SMC_Type *base, uint8_t allowedModes)
{
    base->PMPROT = allowedModes;
}

/*!
 * @brief Gets the current power mode status.
 *
 * This function  returns the current power mode status. After the application
 * switches the power mode, it should always check the status to check whether it
 * runs into the specified mode or not. The application  should  check
 * this mode before switching to a different mode. The system  requires that
 * only certain modes can switch to other specific modes. See the
 * reference manual for details and the smc_power_state_t for information about
 * the power status.
 *
 * @param base SMC peripheral base address.
 * @return Current power mode status.
 */
static inline smc_power_state_t SMC_GetPowerModeState(SMC_Type *base)
{
    return (smc_power_state_t)base->PMSTAT;
}

/*!
 * @brief Prepares to enter stop modes.
 *
 * This function should be called before entering STOP/VLPS/LLS/VLLS modes.
 */
void SMC_PreEnterStopModes(void);

/*!
 * @brief Recovers after wake up from stop modes.
 *
 * This function should be called after wake up from STOP/VLPS/LLS/VLLS modes.
 * It is used with @ref SMC_PreEnterStopModes.
 */
void SMC_PostExitStopModes(void);

/*!
 * @brief Prepares to enter wait modes.
 *
 * This function should be called before entering WAIT/VLPW modes.
 */
void SMC_PreEnterWaitModes(void);

/*!
 * @brief Recovers after wake up from stop modes.
 *
 * This function should be called after wake up from WAIT/VLPW modes.
 * It is used with @ref SMC_PreEnterWaitModes.
 */
void SMC_PostExitWaitModes(void);

/*!
 * @brief Configures the system to RUN power mode.
 *
 * @param base SMC peripheral base address.
 * @return SMC configuration error code.
 */
status_t SMC_SetPowerModeRun(SMC_Type *base);

/*!
 * @brief Configures the system to HSRUN power mode.
 *
 * @param base SMC peripheral base address.
 * @return SMC configuration error code.
 */
status_t SMC_SetPowerModeHsrun(SMC_Type *base);

/*!
 * @brief Configures the system to WAIT power mode.
 *
 * @param base SMC peripheral base address.
 * @return SMC configuration error code.
 */
status_t SMC_SetPowerModeWait(SMC_Type *base);

/*!
 * @brief Configures the system to Stop power mode.
 *
 * @param base SMC peripheral base address.
 * @param  option Partial Stop mode option.
 * @return SMC configuration error code.
 */
status_t SMC_SetPowerModeStop(SMC_Type *base, smc_partial_stop_option_t option);

/*!
 * @brief Configures the system to VLPR power mode.
 *
 * @param base SMC peripheral base address.
 * @return SMC configuration error code.
 */
status_t SMC_SetPowerModeVlpr(SMC_Type *base);

/*!
 * @brief Configures the system to VLPW power mode.
 *
 * @param base SMC peripheral base address.
 * @return SMC configuration error code.
 */
status_t SMC_SetPowerModeVlpw(SMC_Type *base);

/*!
 * @brief Configures the system to VLPS power mode.
 *
 * @param base SMC peripheral base address.
 * @return SMC configuration error code.
 */
status_t SMC_SetPowerModeVlps(SMC_Type *base);



/*@}*/

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/*! @}*/

#endif /* _FSL_SMC_H_ */
