#include <stdarg.h>
#include <stdlib.h>
#if defined(__CC_ARM)
#include <stdio.h>
#endif

#include "fsl_debug_console.h"
#include "fsl_debug_console_conf.h"
#include "fsl_log.h"
#include "fsl_str.h"

/*!
 * @brief This is a printf call back function which is used to relocate the log to buffer
 * or print the log immediately when the local buffer is full.
 *
 * @param[in] buf   Buffer to store log.
 * @param[in] indicator Buffer index.
 * @param[in] val Target character to store.
 * @param[in] len length of the character
 *
 */
#if SDK_DEBUGCONSOLE
static void DbgConsole_RelocateLog(char *buf, int32_t *indicator, char val, int len);
#endif
/*******************************************************************************
 * Code
 ******************************************************************************/

/*************Code for DbgConsole Init, Deinit, Printf, Scanf *******************************/

/* See fsl_debug_console.h for documentation of this function. */
status_t DbgConsole_Init(uint32_t baseAddr, uint32_t baudRate, uint8_t device, uint32_t clkSrcFreq)
{
    assert(device != DEBUG_CONSOLE_DEVICE_TYPE_NONE);

    return LOG_Init(baseAddr, device, baudRate, clkSrcFreq);
}

/* See fsl_debug_console.h for documentation of this function. */
status_t DbgConsole_Deinit(void)
{
    /* LOG deinit */
    LOG_Deinit();

    return kStatus_Success;
}

status_t DbgConsole_Flush(void)
{
    /* wait log and io idle */
    return LOG_WaitIdle();
}

#if SDK_DEBUGCONSOLE
/* See fsl_debug_console.h for documentation of this function. */
int DbgConsole_Printf(const char *fmt_s, ...)
{
    va_list ap;
    int logLength = 0U, result = 0U;
    char printBuf[DEBUG_CONSOLE_PRINTF_MAX_LOG_LEN] = {0U};

    va_start(ap, fmt_s);
    /* format print log first */
    logLength = StrFormatPrintf(fmt_s, ap, printBuf, DbgConsole_RelocateLog);
    /* print log */
    result = LOG_Push((uint8_t *)printBuf, logLength);

    va_end(ap);

    return result;
}

/* See fsl_debug_console.h for documentation of this function. */
int DbgConsole_Putchar(int ch)
{
    /* print char */
    return LOG_Push((uint8_t *)&ch, 1U);
}

/* See fsl_debug_console.h for documentation of this function. */
int DbgConsole_Scanf(char *fmt_ptr, ...)
{
    va_list ap;
    int result;
    char scanfBuf[DEBUG_CONSOLE_SCANF_MAX_LOG_LEN + 1U] = {0U};

    /* scanf log */
    LOG_ReadLine((uint8_t *)scanfBuf, DEBUG_CONSOLE_SCANF_MAX_LOG_LEN);
    /* get va_list */
    va_start(ap, fmt_ptr);
    /* format scanf log */
    result = StrFormatScanf(scanfBuf, fmt_ptr, ap);

    va_end(ap);

    return result;
}

#ifdef DEBUG_CONSOLE_TRANSFER_NON_BLOCKING
status_t DbgConsole_TryGetchar(char *ch)
{
    if (NULL != ch)
    {
        return LOG_TryReadCharacter((uint8_t *)ch);
    }

    return kStatus_Fail;
}
#endif

/* See fsl_debug_console.h for documentation of this function. */
int DbgConsole_Getchar(void)
{
    uint8_t ch;

    /* Get char */
    LOG_ReadCharacter(&ch);

    return ch;
}

static void DbgConsole_RelocateLog(char *buf, int32_t *indicator, char val, int len)
{
    int i = 0;

    for (i = 0; i < len; i++)
    {
        if ((*indicator + 1) >= DEBUG_CONSOLE_PRINTF_MAX_LOG_LEN)
        {
            LOG_Push((uint8_t *)buf, *indicator);
            *indicator = 0U;
        }

        buf[*indicator] = val;
        (*indicator)++;
    }
}

#endif /* SDK_DEBUGCONSOLE */
/*************Code to support toolchain's printf, scanf *******************************/
/* These function __write and __read is used to support ARM_GCC, KDS, Atollic toolchains to printf and scanf*/
#if defined(__GNUC__)

int __attribute__((weak)) _write(int handle, char *buffer, int size)
{
    if (buffer == 0)
    {
        /* return -1 if error. */
        return -1;
    }

    /* This function only writes to "standard out" and "standard err" for all other file handles it returns failure. */
    if ((handle != 1) && (handle != 2))
    {
        return -1;
    }

    /* Send data. */
    LOG_Push((uint8_t *)buffer, size);

    return size;
}

int __attribute__((weak)) _read(int handle, char *buffer, int size)
{
    /* This function only reads from "standard in", for all other file handles it returns failure. */
    if (handle != 0)
    {
        return -1;
    }

    /* Receive data. */
    return LOG_ReadLine((uint8_t *)buffer, size);
}
#endif

