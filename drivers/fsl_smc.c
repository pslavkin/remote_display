#include "fsl_smc.h"
#include "fsl_flash.h"
#include "fsl_common.h"

static uint32_t g_savedPrimask;

void SMC_GetParam(SMC_Type *base, smc_param_t *param)
{
    uint32_t reg = base->PARAM;
    param->hsrunEnable = (bool)(reg & SMC_PARAM_EHSRUN_MASK);
    param->llsEnable = (bool)(reg & SMC_PARAM_ELLS_MASK);
    param->lls2Enable = (bool)(reg & SMC_PARAM_ELLS2_MASK);
    param->vlls0Enable = (bool)(reg & SMC_PARAM_EVLLS0_MASK);
}

void SMC_PreEnterStopModes(void)
{
    flash_prefetch_speculation_status_t speculationStatus = {
        kFLASH_prefetchSpeculationOptionDisable, /* Disable instruction speculation.*/
        kFLASH_prefetchSpeculationOptionDisable, /* Disable data speculation.*/
    };

    g_savedPrimask = DisableGlobalIRQ();
    __ISB();

    /*
     * Before enter stop modes, the flash cache prefetch should be disabled.
     * Otherwise the prefetch might be interrupted by stop, then the data and
     * and instruction from flash are wrong.
     */
    FLASH_PflashSetPrefetchSpeculation(&speculationStatus);
}

void SMC_PostExitStopModes(void)
{
    flash_prefetch_speculation_status_t speculationStatus = {
        kFLASH_prefetchSpeculationOptionEnable, /* Enable instruction speculation.*/
        kFLASH_prefetchSpeculationOptionEnable, /* Enable data speculation.*/
    };

    FLASH_PflashSetPrefetchSpeculation(&speculationStatus);

    EnableGlobalIRQ(g_savedPrimask);
    __ISB();
}

void SMC_PreEnterWaitModes(void)
{
    g_savedPrimask = DisableGlobalIRQ();
    __ISB();
}

void SMC_PostExitWaitModes(void)
{
    EnableGlobalIRQ(g_savedPrimask);
    __ISB();
}

status_t SMC_SetPowerModeRun(SMC_Type *base)
{
    uint8_t reg;

    reg = base->PMCTRL;
    /* configure Normal RUN mode */
    reg &= ~SMC_PMCTRL_RUNM_MASK;
    reg |= (kSMC_RunNormal << SMC_PMCTRL_RUNM_SHIFT);
    base->PMCTRL = reg;

    return kStatus_Success;
}

status_t SMC_SetPowerModeHsrun(SMC_Type *base)
{
    uint8_t reg;

    reg = base->PMCTRL;
    /* configure High Speed RUN mode */
    reg &= ~SMC_PMCTRL_RUNM_MASK;
    reg |= (kSMC_Hsrun << SMC_PMCTRL_RUNM_SHIFT);
    base->PMCTRL = reg;

    return kStatus_Success;
}

status_t SMC_SetPowerModeWait(SMC_Type *base)
{
    /* configure Normal Wait mode */
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    __DSB();
    __WFI();
    __ISB();

    return kStatus_Success;
}

status_t SMC_SetPowerModeStop(SMC_Type *base, smc_partial_stop_option_t option)
{
    uint8_t reg;

    /* configure the Partial Stop mode in Noraml Stop mode */
    reg = base->STOPCTRL;
    reg &= ~SMC_STOPCTRL_PSTOPO_MASK;
    reg |= ((uint32_t)option << SMC_STOPCTRL_PSTOPO_SHIFT);
    base->STOPCTRL = reg;

    /* configure Normal Stop mode */
    reg = base->PMCTRL;
    reg &= ~SMC_PMCTRL_STOPM_MASK;
    reg |= (kSMC_StopNormal << SMC_PMCTRL_STOPM_SHIFT);
    base->PMCTRL = reg;

    /* Set the SLEEPDEEP bit to enable deep sleep mode (stop mode) */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    /* read back to make sure the configuration valid before enter stop mode */
    (void)base->PMCTRL;
    __DSB();
    __WFI();
    __ISB();

    /* check whether the power mode enter Stop mode succeed */
    if (base->PMCTRL & SMC_PMCTRL_STOPA_MASK)
    {
        return kStatus_SMC_StopAbort;
    }
    else
    {
        return kStatus_Success;
    }
}

status_t SMC_SetPowerModeVlpr(SMC_Type *base
                              )
{
    uint8_t reg;

    reg = base->PMCTRL;

    /* configure VLPR mode */
    reg &= ~SMC_PMCTRL_RUNM_MASK;
    reg |= (kSMC_RunVlpr << SMC_PMCTRL_RUNM_SHIFT);
    base->PMCTRL = reg;

    return kStatus_Success;
}

status_t SMC_SetPowerModeVlpw(SMC_Type *base)
{
    /* configure VLPW mode */
    /* Set the SLEEPDEEP bit to enable deep sleep mode */
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    __DSB();
    __WFI();
    __ISB();

    return kStatus_Success;
}

status_t SMC_SetPowerModeVlps(SMC_Type *base)
{
    uint8_t reg;

    /* configure VLPS mode */
    reg = base->PMCTRL;
    reg &= ~SMC_PMCTRL_STOPM_MASK;
    reg |= (kSMC_StopVlps << SMC_PMCTRL_STOPM_SHIFT);
    base->PMCTRL = reg;

    /* Set the SLEEPDEEP bit to enable deep sleep mode */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    /* read back to make sure the configuration valid before enter stop mode */
    (void)base->PMCTRL;
    __DSB();
    __WFI();
    __ISB();

    /* check whether the power mode enter VLPS mode succeed */
    if (base->PMCTRL & SMC_PMCTRL_STOPA_MASK)
    {
        return kStatus_SMC_StopAbort;
    }
    else
    {
        return kStatus_Success;
    }
}


