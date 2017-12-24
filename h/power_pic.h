#ifndef POWER
#define POWER

//------------------------------------------------------
#define SLIDE_LENGTH 9
#define MAX_CHANNELS 4
//------------------------------------------------------
struct Struct_Power
{
	unsigned char Wished;				//potencia desaeada
	unsigned char Actual;				//potencia actual
	unsigned int  Rise_Step;			//guarda el paso de rampa de subida * 512 (uso el 512 a modo de punto fijo, inflando el valor de potencia que va de 0 a 100 y lo inflo antes de dividir por el tiempo de rampa para consirar mayor precision...sino quedaba muy pedorro... luego cuando tengo que asignar la potencia en cada paso de la rampa lo vuelvo a dividir por 512, pero no pierdo el acumulado del error... habria que coordinar con los saltos de tpm para aprovechar todo el espectro...)
	unsigned int  Fall_Step;			//guarda el paso de rampa de bajada * 512 (uso el 512 a modo de punto fijo, inflando el valor de potencia que va de 0 a 100 y lo inflo antes de dividir por el tiempo de rampa para consirar mayor precision...sino quedaba muy pedorro... luego cuando tengo que asignar la potencia en cada paso de la rampa lo vuelvo a dividir por 512, pero no pierdo el acumulado del error... habria que coordinar con los saltos de tpm para aprovechar todo el espectro...)
	unsigned int  Acc_Step;				//lo uso para llevar la potencia actual *512 mientras estoy en las rampas....
	void (*Tpm_Power) (unsigned char Power);	//funcion de tpm para converir potencia en tpm
};
//------------------------------------------------------
extern void 			Add_Power					(void);	
extern void 			Del_Power					(void);	
extern struct Struct_Pic 	Power_Pic;
extern void 			Pasive_Power					(void);
extern void 			Active_Power					(void);
extern void 			Inc_Power_Ch0					(void);
extern void 			Inc_Power_Ch1					(void);
extern void 			Inc_Power_Ch2					(void);
extern void 			Inc_Power_Ch3					(void);
extern void 			Dec_Power_Ch0					(void);
extern void 			Dec_Power_Ch1					(void);
extern void 			Dec_Power_Ch2					(void);
extern void 			Dec_Power_Ch3					(void);
extern void 			Inc_Power_Ch_All				(void);
extern void 			Dec_Power_Ch_All				(void);
extern void 			Update_All_Power_Ch				(void);
extern void 			Ramp_Up_Power					(unsigned char Ch);
extern void 			Ramp_Up_Power_All				(void);
extern void 			Ramp_Down_Power					(unsigned char Ch);
extern void 			Ramp_Down_Power_All				(void);
extern void 			Assign_Tpm_Power_Func				(void);
extern void 			Update_Rise_Step				(unsigned char Ch);
extern void 			Update_Fall_Step				(unsigned char Ch);
extern void 			Update_Rise_Step_All				(void);
extern void 			Update_Fall_Step_All				(void);
extern void 			Set_Actual_Power_All				(unsigned char Power);
extern void 			Wished_Power2Actual				(void);
extern void 			Update_Power_Ch					(unsigned char Ch);
//------------------------------------------------------
extern void 			Play_Stop						(void);
extern void 			Move_Pointer2Power_All_And_Actual_Pic_Handler_Index2Up	(void);	
extern void 			Move_Pointer2Powe_1_And_Actual_Pic_Handler_Index2Down	(void);	
extern void 			Move_Pointer2Power_4_And_Actual_Pic_Handler_Index2Up	(void);	
extern void 			Move_Pointer2Power_All_And_Set_Handler2Powe_All		(void);	
extern void 			Move_Pointer2Time_And_Actual_Pic_Handler_Index2Down	(void);	
extern void 			Move_Pointer2Player_And_Actual_Pic_Handler_Index2Down 	(void);	
extern void 			Move_Pointer2Time_And_Actual_Pic_Handler_Index2Up	(void);	
extern void 			V7Scroll2Up_And_Dont_Blink_Pointer			(void);	
extern void 			V7Scroll2Down_And_Blink_Pointer				(void);	
extern void 			Pasive_Power_And_Active_Time_Param			(void);	
extern void			Del_All_Layers_But_Power				(void);
//------------------------------------------------------
#endif
