#include "mke18f16.h"
#include "fsl_clock.h"
#include "fsl_port.h"
#include <serial_phisical.h>
#include <serial_tx.h>
#include <serial_rx.h>
#include <state_machine.h>
#include <events.h>
#include <serial_session.h>
#include <everythings.h>


#define Enable_Tx_Irq()		(LPUART0->CTRL|=LPUART_CTRL_TIE_MASK)				//hay lugar...
#define Enable_Complete_Irq()	(LPUART0->CTRL|=LPUART_CTRL_TCIE_MASK)				//enviado hastra la ultima miga..
#define Disable_Tx_Irq()	(LPUART0->CTRL&=~(LPUART_CTRL_TIE_MASK|LPUART_CTRL_TCIE_MASK))	//apga todo...
//--------------------------------------------------------------------------
void Serial_Phisical_Wait_Last_Byte	(void) {Enable_Complete_Irq();}
//--------------------------------------------------------------------------
void Init_Serial_Phisical(void)
{
	CLOCK_EnableClock(kCLOCK_PortB);
	/* PORTB0 (pin 54) is configured as LPUART0_RX */
	PORT_SetPinMux(PORTB, 0U, kPORT_MuxAlt2);
	/* PORTB1 (pin 53) is configured as LPUART0_TX */
	PORT_SetPinMux(PORTB, 1U, kPORT_MuxAlt2);
	CLOCK_SetIpSrc(kCLOCK_Lpuart0, kCLOCK_IpSrcSysPllAsync);	//habilito clk al periferico y le mando 60Mhz
	CLOCK_EnableClock(kCLOCK_Lpuart0);
	LPUART0->GLOBAL |= LPUART_GLOBAL_RST_MASK; 			//resetea
	LPUART0->GLOBAL &= ~LPUART_GLOBAL_RST_MASK; 			//resetea
	LPUART0->BAUD=0x0F000000|390;  				//60Mhz / 16*9600 = 390
	LPUART0->CTRL|=LPUART_CTRL_TE_MASK|LPUART_CTRL_RE_MASK|LPUART_CTRL_RIE_MASK; //habilito tx, rx y interrupciones en rx
	NVIC_EnableIRQ(LPUART0_RX_IRQn);
	NVIC_EnableIRQ(LPUART0_TX_IRQn);
}
//--------------------------------------------------------------------------
void Serial_Phisical_Tx(unsigned char Data)	//esta funcion envia un byte por el serial...
{
	// while(!SCI2_S1_TDRE);				//espero a que haya lugar (y ademas limpio el flag de 'hay espacio'
	LPUART0->DATA=Data;					//simplemente manda el dato...
	Enable_Tx_Irq();				//habilita la irq para que cuando haya espacio avise...
}
//--------------------------------------------------------------------------
void LPUART0_TX_IRQHandler(void)
{
	Disable_Tx_Irq();				//enmascara la irq...
	Send_Event(Byte_Ready_Event,Serial_Tx());	//lo unico que hace la irq es mandar un mensaje a la maquina de que esta lista para que esta decida lo que hacer....
}
void LPUART0_RX_IRQHandler(void)
{
	Send_Event((unsigned char)LPUART0->DATA,Data_Serial_Rx());				//al leer limpia el flag automaticamente
}
//--------------------------------------------------------------------------

