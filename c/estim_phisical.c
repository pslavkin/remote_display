#include "everythings.h"
#include "estim_phisical.h"
#include "events.h"
#include "power_pic.h"
#include "tpm.h"
#include "serial_tx.h"
#include "setting_menu_pic.h"
#include "programs_pic.h"
#include "programs_pic.h"
#include "debug.h"
#include "type_conversion.h"

//-------------------------------------------------------------
static State  
	Off[],
	  A[],
	  D[], 
	  E[],
	  H[],
	  F[];
//-------------------------------------------------------------
unsigned int 	Fire_Counter;
unsigned char 	Secuence_Counter;
unsigned char 	Channel_Sequence_Counter;
State* 		Estim_Phisical_Sm;
//-------------------------------------------------------------
void 	Init_Estim_Phisical	(void)	
{
 	Estim_Phisical_Sm=Off;
	Init_Setting_Menu();
	Init_Ftm0();		//primero levanto los ftm's
	Init_Ftm1();	
	Init_Programs();	//despues inicializo los programas que hacen uso de los ftm's
}
//----------------------------------------------------------------------------------------------------
void Off_Start		(void)
{
	Send_NVDebug_Estim_Phisical_Data2Serial(11,"Off Start\r\n");
	Set_Bank_In_Use	(0);			//me copa arrancar siempre con el primer banco... ademas imaginate que el programa no tiene segundo banco....
	Set_Actual_Power_All(0);
	Fire_Counter=0;				//cuenta desde cero hasta Read_E()
	Secuence_Counter=0;			//cuenta desde cero hasta Read_G() con lo cual si cambia G en tiempo de ejecucion, automaticamente se entera,,,
	Channel_Sequence_Counter=0;
	Update_Rise_Step_All();
	Ramp_Up_Power_All();
}
//--------------------
void A_Rti	(void)
{
	Send_NVDebug_Estim_Phisical_Data2Serial(18,"Ramping up A_Rti\r\n");
	if(++Fire_Counter>=Read_A()) {
	 	Atomic_Send_Event(D_Event,Estim_Phisical());
	}
	else 	Ramp_Up_Power_All();
}
void A_D	(void)	
{
	Fire_Counter=0;						//cuenta desde cero hasta Read_D()	
 	Send_NVDebug_Estim_Phisical_Data2Serial(20,"Fin ramping up A_D\r\n");
	Wished_Power2Actual();
}
//--------------------
void D_Rti	(void)
{
	Send_NVDebug_Estim_Phisical_Data2Serial(19,"Contraction D_Rti\r\n");
	unsigned char D=Read_D();
	Wished_Power2Actual();
	if(D && ++Fire_Counter>=(D*10)) 					//d=0 sigo for ever.. en otro caso, compara con d*10, que son los segundos de contraccion...	
		Atomic_Send_Event(E_Event,Estim_Phisical());	
}
void D_E	(void)
{
 	Send_NVDebug_Estim_Phisical_Data2Serial(17,"Fin working D_E\r\n");
	Fire_Counter=0;							//cuenta desde cero hasta Read_E()
	Update_Fall_Step_All();
	Ramp_Down_Power_All();
}
//--------------------
void E_Rti	(void)
{
	Send_NVDebug_Estim_Phisical_Data2Serial(20,"Ramping down E_Rti\r\n");
	if(++Fire_Counter>=Read_E()) 
		Atomic_Send_Event(H_Event,Estim_Phisical());
	else {
		Ramp_Down_Power_All();
	}
}
void E_H	(void)
{
 	Send_NVDebug_Estim_Phisical_Data2Serial(22,"Fin ramping down E_F\r\n");
	Set_Actual_Power_All(0);					//como pueden haber cambiado la potencia en tiempo de ejecucion podria darse el caso de que no haya llegado a cero cuando hace la rampa.. porque la rama la va bajando en funcion del valor de wished power y la cantidad de disparos... si eso me lo cambian mientrass lo estoy moviendo puede que quede con algo de power...con esto me aseguro de que no...
}
//--------------------
void H_Rti	(void)
{
	Send_NVDebug_Estim_Phisical_Data2Serial(20,"Channel effects H_Rti\r\n");
	if(Next_Sequence_Channel() && Read_I()!=0 && ++Channel_Sequence_Counter>=Read_I())  //compara el numero de secuenci actual con el solicitado, siempre y cuando sea distinto de cero...en ese caso se queda para siempre
		Atomic_Send_Event(F_Event,Estim_Phisical());
	else 
		Atomic_Send_Event(A_Event,Estim_Phisical());
 	Send_NVDebug_Estim_Phisical_Data2Serial(19,"Next Sequence Data=");
 	Send_NVDebug_Estim_Phisical_Char_NLine2Serial(Actual_Sequence_Data());
}
void H_F	(void)
{
 	Send_NVDebug_Estim_Phisical_Data2Serial(26,"Fin Channel Sequence H_F\r\n");
}
void H_A	(void)
{
 	Send_NVDebug_Estim_Phisical_Data2Serial(27,"Next Sequence Channel H_A\r\n");
	Update_Rise_Step_All();
	Ramp_Up_Power_All();
	Fire_Counter=0;							//cuenta desde cero hasta Read_F()
}
//--------------------
void F_Rti	(void)
{
	unsigned int F=Read_F()*10;
	Fire_Counter++;
	F=F>Fire_Counter?(F-Fire_Counter):0;					//calculo cuanto me falta para terminar.. sin numeros negativos..pensa que lo podria estar cambiando on the fly
	Send_NVDebug_Estim_Phisical_Data2Serial(16,"Relaxing F_Rti\r\n");
		if(F==0) 	Atomic_Send_Event(G_Event,Estim_Phisical());
	else 	if(F<=50) 	{}//Set_Temp_Led_Effect(Led_Blue_Index,0x0001);
}

void F_G	(void)
{
 	Send_NVDebug_Estim_Phisical_Data2Serial(20,"Fin relajacion F_G\r\n");
	if(Read_G()!=0 && ++Secuence_Counter>=Read_G())	{
		Send_NVDebug_Estim_Phisical_Data2Serial(17,"Cambio de banco\r\n");
		Toogle_Bank();					//cambia de parametros al otro grupo. a partir de ahora todo sigue igual solo que con otro numeros...
		Update_Bank_Parameters();			//actualizo los parametros de frec, pulso y fase..
		Secuence_Counter=0;				//arranca en cero como siempre hasta Read_G
	}
	Channel_Sequence_Counter=0;
	Fire_Counter=0;
	Update_Rise_Step_All();
	Ramp_Up_Power_All();
}
//--------------------
void Anywere_Off	(void) 
{
	Send_NVDebug_Estim_Phisical_Data2Serial(10,"Turn Off\r\n");
	Set_Actual_Power_All(0);
}
//----------------------------------------------------------------------------------------------------
void 		Turn_Off_Estim_Phisical		(void) 	{Atomic_Send_Event(Stop_Event,Estim_Phisical());}
void 		Turn_On_Estim_Phisical 		(void)  {Atomic_Send_Event(Start_Event,Estim_Phisical());}
unsigned char 	Estim_Phisical_State		(void)	{return Estim_Phisical_Sm!=Off;}
//----------------------------------------------------------------------------------------------------
State** 	Estim_Phisical			(void) {return &Estim_Phisical_Sm;}
void 		Estim_Phisical_Rti		(void) {Atomic_Send_Event(Rti_Event,Estim_Phisical());}
//----------------------------------------------------------------------------------------------------
static State Off[] RODATA =
{
{ Start_Event			,Off_Start			,A},
{ ANY_Event                      ,Rien			 	,Off}, 
};
static State A[] RODATA = 
{
{ D_Event			,A_D 				,D},
{ Rti_Event			,A_Rti				,A},
{ Stop_Event			,Anywere_Off			,Off},
{ ANY_Event			,Rien				,A},
};
static State D[] RODATA = 
{
{ E_Event			,D_E 				,E},
{ Rti_Event			,D_Rti				,D},
{ Stop_Event			,Anywere_Off			,Off},
{ ANY_Event			,Rien				,D},
};
static State E[] RODATA = 
{
{ H_Event			,E_H 				,H},
{ Rti_Event			,E_Rti				,E},
{ Stop_Event			,Anywere_Off			,Off},
{ ANY_Event			,Rien				,E},
};
static State H[] RODATA = 
{
{ A_Event			,H_A 				,A},
{ F_Event			,H_F 				,F},
{ Rti_Event			,H_Rti				,H},
{ Stop_Event			,Anywere_Off			,Off},
{ ANY_Event			,Rien				,H},
};
static State F[] RODATA = 
{
{ G_Event			,F_G 				,A},
{ Rti_Event			,F_Rti				,F},
{ Stop_Event			,Anywere_Off			,Off},
{ ANY_Event			,Rien				,F},
};
//----------------------------------------------------------------------------------------------------

