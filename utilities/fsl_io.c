#include "fsl_io.h"
#include "fsl_debug_console_conf.h"

/* check avaliable device  */
#define DEBUG_CONSOLE_IO_LPUART

/* configuration for debug console device */
/* If new device is required as the low level device for debug console,
 * Add the #elif branch and add the preprocessor macro to judge whether
 * this kind of device exist in this SOC. */

#include "fsl_lpuart.h"
#ifdef DEBUG_CONSOLE_TRANSFER_NON_BLOCKING
static lpuart_handle_t s_ioLpuartHandler;
#endif /* DEBUG_CONSOLE_TRANSFER_NON_BLOCKING */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Debug console IO state information. */
static io_state_t s_debugConsoleIO = {
    .ioBase = NULL,
    .ioType = DEBUG_CONSOLE_DEVICE_TYPE_NONE,
#ifdef DEBUG_CONSOLE_TRANSFER_NON_BLOCKING
    .callBack = NULL,
#endif /* DEBUG_CONSOLE_TRANSFER_NON_BLOCKING */
};

/*******************************************************************************
 * Code
 ******************************************************************************/

#ifdef DEBUG_CONSOLE_TRANSFER_NON_BLOCKING

static void LPUART_Callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{
    bool tx = false, rx = false;
    size_t size = 0U;

    if (status == kStatus_LPUART_RxIdle)
    {
        rx = true;
        size = handle->txDataSizeAll;
    }

    if (status == kStatus_LPUART_TxIdle)
    {
        tx = true;
        size = handle->txDataSizeAll;
    }

    /* inform the buffer layer that transfer is complete */
    if (s_debugConsoleIO.callBack != NULL)
    {
        /* call buffer callback function */
        s_debugConsoleIO.callBack(&size, rx, tx);
    }
}
#endif /* DEBUG_CONSOLE_TRANSFER_NON_BLOCKING */

void IO_Init(io_state_t *io, uint32_t baudRate, uint32_t clkSrcFreq, uint8_t *ringBuffer)
{
    assert(NULL != io);

    /* record device type/base */
    s_debugConsoleIO.ioType = io->ioType;
    s_debugConsoleIO.ioBase = (void *)(io->ioBase);

    switch (s_debugConsoleIO.ioType)
    {
        case DEBUG_CONSOLE_DEVICE_TYPE_LPUART:
        {
            lpuart_config_t lpuart_config;
            LPUART_GetDefaultConfig(&lpuart_config);
            lpuart_config.baudRate_Bps = baudRate;
            /* Enable clock and initial UART module follow user configure structure. */
            LPUART_Init((LPUART_Type *)s_debugConsoleIO.ioBase, &lpuart_config, clkSrcFreq);
            LPUART_EnableTx(s_debugConsoleIO.ioBase, true);
            LPUART_EnableRx(s_debugConsoleIO.ioBase, true);
#ifdef DEBUG_CONSOLE_TRANSFER_NON_BLOCKING
            s_debugConsoleIO.callBack = io->callBack;
            /* create handler for interrupt transfer */
            LPUART_TransferCreateHandle(s_debugConsoleIO.ioBase, &s_ioLpuartHandler, LPUART_Callback, NULL);
            /* start ring buffer */
            LPUART_TransferStartRingBuffer(s_debugConsoleIO.ioBase, &s_ioLpuartHandler, ringBuffer,
                                           DEBUG_CONSOLE_RECEIVE_BUFFER_LEN);
#endif
        }
        break;
    }
}

status_t IO_Deinit(void)
{
    if (s_debugConsoleIO.ioType == DEBUG_CONSOLE_DEVICE_TYPE_NONE)
    {
        return kStatus_Success;
    }

    switch (s_debugConsoleIO.ioType)
    {
        case DEBUG_CONSOLE_DEVICE_TYPE_LPUART:
#ifdef DEBUG_CONSOLE_TRANSFER_NON_BLOCKING
            /* stop ring buffer */
            LPUART_TransferStopRingBuffer(s_debugConsoleIO.ioBase, &s_ioLpuartHandler);
#endif
            /* Disable LPUART module. */
            LPUART_Deinit((LPUART_Type *)s_debugConsoleIO.ioBase);

            break;
        default:
            s_debugConsoleIO.ioType = DEBUG_CONSOLE_DEVICE_TYPE_NONE;
            break;
    }

    s_debugConsoleIO.ioType = DEBUG_CONSOLE_DEVICE_TYPE_NONE;

    return kStatus_Success;
}

status_t IO_WaitIdle(void)
{
    switch (s_debugConsoleIO.ioType)
    {
        case DEBUG_CONSOLE_DEVICE_TYPE_LPUART:
            /* wait transfer complete flag */
            while (!(LPUART_GetStatusFlags(s_debugConsoleIO.ioBase) & kLPUART_TransmissionCompleteFlag))
                ;
            break;
        default:
            break;
    }
    return kStatus_Success;
}

#ifdef DEBUG_CONSOLE_TRANSFER_NON_BLOCKING

status_t IO_Transfer(uint8_t *ch, size_t size, bool tx)
{
    status_t status = kStatus_Fail;

    switch (s_debugConsoleIO.ioType)
    {
        case DEBUG_CONSOLE_DEVICE_TYPE_LPUART:
        {
            lpuart_transfer_t transfer = {0U};
            transfer.data = ch;
            transfer.dataSize = size;
            /* transfer data */
            if (tx)
            {
                status = LPUART_TransferSendNonBlocking(s_debugConsoleIO.ioBase, &s_ioLpuartHandler, &transfer);
            }
            else
            {
                status =
                    LPUART_TransferReceiveNonBlocking(s_debugConsoleIO.ioBase, &s_ioLpuartHandler, &transfer, NULL);
            }
        }
        break;
        default:
            break;
    }
    return status;
}

status_t IO_TryReceiveCharacter(uint8_t *ch)
{
    status_t status = kStatus_Fail;
    uint32_t size = 1U;

    switch (s_debugConsoleIO.ioType)
    {
        case DEBUG_CONSOLE_DEVICE_TYPE_LPUART:
        {
            lpuart_transfer_t transfer = {0U};
            transfer.data = ch;
            transfer.dataSize = size;
            if (LPUART_TransferGetRxRingBufferLength(s_debugConsoleIO.ioBase, &s_ioLpuartHandler) >= size)
            {
                /* transfer data */
                status =
                    LPUART_TransferReceiveNonBlocking(s_debugConsoleIO.ioBase, &s_ioLpuartHandler, &transfer, NULL);
            }
        }
        break;
        default:
            break;
    }
    return status;
}

#else

status_t IO_Transfer(uint8_t *ch, size_t size, bool tx)
{
    status_t status = kStatus_Success;
    switch (s_debugConsoleIO.ioType)
    {
        case DEBUG_CONSOLE_DEVICE_TYPE_LPUART:
        {
            if (tx)
            {
                LPUART_WriteBlocking(s_debugConsoleIO.ioBase, ch, size);
            }
            else
            {
                status = LPUART_ReadBlocking(s_debugConsoleIO.ioBase, ch, size);
            }
        }
        break;
        default:
            status = kStatus_Fail;
            break;
    }

    return status;
}

#endif /* DEBUG_CONSOLE_TRANSFER_NON_BLOCKING */
