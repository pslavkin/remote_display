#ifndef ESTIM_PHISICAL
#define ESTIM_PHISICAL

#include "state_machine.h"

//------------------------------------------------------
enum Estim_Phisical_Event_Code{
			Start_Event			= 0x9000,
			A_Event				= 0x9001,	
			D_Event				= 0x9002,	
			E_Event				= 0x9003,	
			F_Event				= 0x9004,	
			G_Event				= 0x9005,	
			H_Event				= 0x9006,	
			Stop_Event			= 0x9007
};
//------------------------------------------------------
extern void 			Init_Estim_Phisical	(void);
extern State**  		Estim_Phisical		(void);
extern void 			Estim_Phisical_Rti	(void);
//------------------------------------------------------
extern unsigned char 		Estim_Phisical_State	(void);
extern void 			Turn_Off_Estim_Phisical	(void);	
extern void 			Turn_On_Estim_Phisical 	(void);	
//------------------------------------------------------
extern void 			Assign_Tpm_Power_Func	(void);

#endif
