#ifndef SERIAL_PHISICAL 	//SCI0
#define SERIAL_PHISICAL
//------------------------------------------------------------------------------
extern 	void 	Serial_Phisical_Tx		(unsigned char);			
extern 	void 	Init_Serial_Phisical		(void);
extern  void 	LPUART0_TX_IRQHandler		(void);
extern  void 	LPUART0_RX_IRQHandler		(void);
extern 	void 	Serial_Phisical_Wait_Last_Byte	(void);
//------------------------------------------------------------------------------
#endif


