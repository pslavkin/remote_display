#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "mke18f16.h"
#include "fsl_port.h"
#include "events.h"
#include "everythings.h"
#include "state_machine.h"
#include "rti.h"
#include "serial_session.h"
#include "type_conversion.h"

void Wdog_Refresh(void)
{
	WDOG->CNT =  WDOG_REFRESH_KEY;
}
void Wdog_Disable(void)
{
	WDOG->CNT = WDOG_UPDATE_KEY;
	WDOG->TOVAL = 0xFFFF;
	WDOG->CS = (uint32_t) ((WDOG->CS) & ~WDOG_CS_EN_MASK) | WDOG_CS_UPDATE_MASK;
}
void Wdog_Enable(void)
{
	WDOG->CNT = WDOG_UPDATE_KEY;
	WDOG->TOVAL = 0xFFFF;
	WDOG->CS = (uint32_t) ((WDOG->CS) | WDOG_CS_EN_MASK) | WDOG_CS_UPDATE_MASK;
}

int main(void) {
	BOARD_InitBootClocks();
	Wdog_Disable();
	Init_Events();
	Init_Serial_Session();
	Init_Everythings();
	Init_Rti();
	Enable_Irq();
	for(;;Wdog_Refresh()) 
		State_Machine();
    return 0 ;
}
