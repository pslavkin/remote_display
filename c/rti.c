#include "rti.h"
#include "everythings.h"
#include "mke18f16.h"
#include "fsl_clock.h"
#include "display_phisical.h"
//---------------------------------------------------
void Func1(void);
void Func2(void);
void Func3(void);
void Func4(void);
void Func5(void);
//---------------------------------------------------
void (*Rti) (void);
//---------------------------------------------------
void Func1(void)
{
 Everythings_Rti();
 Rti=Func2;
}
void Func2(void)
{
 Rti=Func3;
}
void Func3(void)
{
 Rti=Func4;
}
void Func4(void)
{
 Rti=Func5;
}
void Func5(void)
{
 Rti=Func1;
}
//---------------------------------------------------
void Init_Rti(void)
{
   Rti=Func1;                                 // arranca ejecutando func1...
   SysTick->LOAD  = 240000;                   // 120Mhz / 500hz = 2mseg sisi, 120 Mhz.. I repeat....
   SysTick->VAL   = 0UL;                      /* Load the SysTick Counter Value */
   SysTick->CTRL  =  SysTick_CTRL_CLKSOURCE_Msk |
                     SysTick_CTRL_TICKINT_Msk   |
                     SysTick_CTRL_ENABLE_Msk; /* Enable SysTick IRQ and SysTick Timer */
}

void SysTick_Handler(void)
{
   Rti();
}


//---------------------------------------------------

