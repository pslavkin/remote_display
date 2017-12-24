#ifndef SERIAL_TX_FIFO
#define SERIAL_TX_FIFO
#include<state_machine.h>
//---------------------------------------------------------------------
struct Serial_Tx_Message
       {
	 unsigned char* Start;
	 unsigned int   Size;
	 unsigned char  Type;	//indica si es de tipo data/address/485_Data o 485_Address..
	 State**	Sm;	//enviara una respuesta ni bien termine a la maquina solicitante...Si no se requiere se coloca Empty....
       };
//---------------------------------------------------------------------
extern void 			Init_Serial_Tx_Fifo 		(void);
extern void 			Atomic_Add_Serial_Tx_Message 	(unsigned char* Start,unsigned int Size,unsigned char Type,State** Sm);
extern struct Serial_Tx_Message Atomic_Read_Serial_Tx_Message 	(void);
extern unsigned char 		Serial_Tx_Fifo_Messages 	(void);
extern unsigned char 		Read_Message_Type 		(void);
//---------------------------------------------------------------------


#endif

