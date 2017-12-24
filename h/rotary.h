#ifndef ROTARY
#define ROTARY

#include <state_machine.h>

//PTDD5=button_rotary
#define INIT_ROTARY_BUTTON()			//PORT_PTDIE_PTDIE5=1,PORT_PTDPE_PTDPE5=1)
#define READ_ROTARY_BUTTON()			//PORT_PTDD&PORT_PTDD_PTDD5_MASK) 

#define ROTARY_FILTER	1			//determina cuantas irqs tienen que pasar para el mismo lado para que se considere valido el movimiento.. es para que si se mueve muy poquito no lo tome porque sino queda muy sensible
#define ROTARY_TOUT     20			//como es codigo gray, tengo que mirar 2 disparos para saber para que lado va.. pero que pasa si viene yno y despue sno viene mas nada por 2 semanas.. cuando venga el proximo yo que se para que lado queria ir.. con lo cual al ratito del primer disparo sino viene el segundo.. lo resetea
#define MAX_ACCEL    	40			//esto define cuandos handlers consecutivos se podrian ejecutar en funcion de la velocidad de giro del rotary

struct Struct_Rotary
{
	  signed char Count;
	unsigned char Accel;
	unsigned char Repetition;
	unsigned long Actual_Tick;
	unsigned long Last_Tick;
};

enum Rotary_Event_Code
{
				Rotary_ACW_Event		=0x0000, //estos valores salen del hardwaremismo para ser mas eficiente.. no se puede elegir cualquiera...son para uso interno solamente..
				Rotary_CW_Event			=0x0040,

				Reset_Rotary_Count_Event	=0x2000,  //inventados

				Rotary_CW_Handler_Event		=0x2100,
				Rotary_ACW_Handler_Event	=0x2200,
				Rotary_CW_Handler_End_Event	=0x2300,
				Rotary_ACW_Handler_End_Event	=0x2400,
				Rotary_ACW_Code 		='1',		//1, 2 y 3	//para mandar pir serie monobyte
				Rotary_CW_Code 			='4'		//4, 5 y 6                                     
};
	
//extern unsigned char	Rotary_ACW_Code_Const;
//extern unsigned char	Rotary_CW_Code_Const; 
//-------------------------------------------------------------------------------------
extern void 			Init_Rotary		(void);
extern State** 			Rotary			(void);
extern void			Rotary_Rti		(void);
//--------------------------------------------------------------------------------------
extern void 			Rotary_Irq		(void);
//--------------------------------------------------------------------------------------
extern unsigned char 		Read_Rotary_Filter	(void);
extern void 			Save_Rotary_Filter	(unsigned char Data);
extern unsigned char 		Read_Rotary_TOut	(void);
extern void 			Save_Rotary_TOut	(unsigned char Data);
extern unsigned char		Read_Rotary_Accel	(void);
extern void	 		Print_Rotary_Accel	(void);
extern void 			Print_Rotary_Sm_Add	(void);
//--------------------------------------------------------------------------------------
#endif
