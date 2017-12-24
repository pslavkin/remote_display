#ifndef STATE_MACHINE
#define STATE_MACHINE

#include <stdint.h>
//------------------------------------------------------------------
typedef struct Efn		//Event-Function-Next
 {
   unsigned int Event;			//ojo que mepa que int son 32 bits... no 16 como creo que lo estoy pensando....
   void (*Func) (void);
   struct Efn *Next_State;
 } State;
//------------------------------------------------------------------
#define Enable_Irq()	__asm volatile ("cpsie i")
#define Disable_Irq() 	__asm volatile ("cpsid i")

#define Atomic(Function) {__asm volatile ("cpsid i") ;Function;__asm volatile ("cpsie i");}
//------------------------------------------------------------------
extern void 		Rien 		(void);
extern void 		Set_State	(State* New_State,State** Machine);
extern unsigned int 	Actual_Event	(void);
extern State**   	Actual_Sm	(void);
extern void 		State_Machine	(void);	//maquina principal de principales que ejecuta la maquina de estados...
extern State** 		Empty_Sm	(void);
extern State* 		Empty_App	(void);
extern void 		Soft_Reset	(void);
extern void 		Delay_Useg	(uint32_t Useg);
extern void 		Pum		(void);
//------------------------------------------------------------------
#define  Empty_State_Machine 		((State**)0x00000000)

enum Event_Code {
		 Empty_Event 			= 0x0000,	//este evento se usa para saber cuando la cola de eventos esta vacia...
		 Request_State_Event		= 0xFFFB,	//se usa para pedirle a una maquina que devuelva el estado actual al solicitante... para lo cal la maquina solicitada tiene que parsear este evento en todos sus estado.. sino no va... y ademas la devolcion debera ser parseada tambien por la maquina que lo solicito siguiento el codigo que se le haya ocurrido a la maquina solicitada...
		 Print_State_Event		= 0xFFFC,	//se usa para debuguear estados... nada mas
		 Invalid_Event			= 0xFFFD,
		 Rti_Event			= 0xFFFE,
		 ANY_Event			= 0xFFFF	//este eventos se usa como default en la comparacion. Es decir si el evento no coincide con ninguno de los posibles, entonces terminara la busqueda cuando encuentre ANY. Notar que es muy imporante que haya un ANY en cada estado para tener por donde salir en el caso de que las comparaciones fallen.
		};
//------------------------------------------------------------------
#endif

