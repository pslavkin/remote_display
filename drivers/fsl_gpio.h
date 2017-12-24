#ifndef _FSL_GPIO_H_
#define _FSL_GPIO_H_

#include "fsl_common.h"

/*! @brief GPIO direction definition */
typedef enum _gpio_pin_direction
{
    kGPIO_DigitalInput = 0U,  /*!< Set current pin as digital input*/
    kGPIO_DigitalOutput = 1U, /*!< Set current pin as digital output*/
} gpio_pin_direction_t;

// API
#if defined(__cplusplus)
extern "C" {
#endif

// @brief Initializes a GPIO pin used by the board.
void GPIO_PinInit_As_Int(GPIO_Type *base, uint32_t pin);
void GPIO_PinInit_As_Out(GPIO_Type *base, uint32_t pin,uint8_t value);



static inline void GPIO_Port_As_Out(GPIO_Type *base, uint32_t mask)
{
        base->PDDR |= mask;
}
static inline void GPIO_Port_As_In(GPIO_Type *base, uint32_t mask)
{
        base->PDDR &= ~mask;
}

/*! @name GPIO Output Operations */
/*!
 * @brief Sets the output level of the multiple GPIO pins to the logic 1 or 0.
 *
 * @param base    GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param pin     GPIO pin number
 * @param output  GPIO pin output logic level.
 *        - 0: corresponding pin output low-logic level.
 *        - 1: corresponding pin output high-logic level.
 */
static inline void GPIO_PinWrite(GPIO_Type *base, uint32_t pin, uint8_t output)
{
    if (output == 0U)
    {
        base->PCOR = 1U << pin;
    }
    else
    {
        base->PSOR = 1U << pin;
    }
}

static inline void GPIO_PortWrite(GPIO_Type *base, uint32_t mask)
{
    base->PSOR = mask;
    base->PCOR = ~mask;
}
static inline uint32_t GPIO_PortRead(GPIO_Type *base)
{
    return base->PDIR;
}
/*!
 * @brief Sets the output level of the multiple GPIO pins to the logic 1.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param mask GPIO pin number macro
 */
static inline void GPIO_PortSet(GPIO_Type *base, uint32_t mask)
{
    base->PSOR = mask;
}

/*!
 * @brief Sets the output level of the multiple GPIO pins to the logic 0.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param mask GPIO pin number macro
 */
static inline void GPIO_PortClear(GPIO_Type *base, uint32_t mask)
{
    base->PCOR = mask;
}


/*!
 * @brief Reverses the current output logic of the multiple GPIO pins.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param mask GPIO pin number macro
 */
static inline void GPIO_PortToggle(GPIO_Type *base, uint32_t mask)
{
    base->PTOR = mask;
}


/*! @name GPIO Input Operations */
/*@{*/

/*!
 * @brief Reads the current input value of the GPIO port.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param pin     GPIO pin number
 * @retval GPIO port input value
 *        - 0: corresponding pin input low-logic level.
 *        - 1: corresponding pin input high-logic level.
 */
static inline uint32_t GPIO_PinRead(GPIO_Type *base, uint32_t pin)
{
    return (((base->PDIR) >> pin) & 0x01U);
}


/*! @name GPIO Interrupt */
/*@{*/

/*!
 * @brief Reads the GPIO port interrupt status flag.
 *
 * If a pin is configured to generate the DMA request, the corresponding flag
 * is cleared automatically at the completion of the requested DMA transfer.
 * Otherwise, the flag remains set until a logic one is written to that flag.
 * If configured for a level sensitive interrupt that remains asserted, the flag
 * is set again immediately.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @retval The current GPIO port interrupt status flag, for example, 0x00010001 means the
 *         pin 0 and 17 have the interrupt.
 */
uint32_t GPIO_PortGetInterruptFlags(GPIO_Type *base);

/*!
 * @brief Reads the GPIO port interrupt status flag.
 * @deprecated Do not use this function.  It has been superceded by @ref GPIO_PortGetInterruptFlags.
 */
static inline uint32_t GPIO_GetPinsInterruptFlags(GPIO_Type *base)
{
    return GPIO_PortGetInterruptFlags(base);
}

/*!
 * @brief Clears multiple GPIO pin interrupt status flags.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param mask GPIO pin number macro
 */
void GPIO_PortClearInterruptFlags(GPIO_Type *base, uint32_t mask);

/*!
 * @brief Clears multiple GPIO pin interrupt status flags.
 * @deprecated Do not use this function.  It has been superceded by @ref GPIO_PortClearInterruptFlags.
 */
static inline void GPIO_ClearPinsInterruptFlags(GPIO_Type *base, uint32_t mask)
{
    GPIO_PortClearInterruptFlags(base, mask);
}


#if defined(__cplusplus)
}
#endif

#endif /* _FSL_GPIO_H_*/
