#include "serial_tx.h"
#include "serial_rx.h"
#include "serial_phisical.h"
#include "serial_session.h"
#include "state_machine.h"
#include "events.h"
#include "type_conversion.h"
#include "schedule.h"
#include "debug.h"
#include "everythings.h"
#include "type_conversion.h"

#define SERIAL_RX_BUFFER  20

State *Data_Serial_Rx_Sm;
State** App_Sm;							//usada para recordar quien solicito grabar datos desde el serial y avisarle oportunamente las novedades...
static unsigned char 	Rx_Buffer[SERIAL_RX_BUFFER];
static unsigned int  	Bytes2Read;
static unsigned char*	Point2Read;				//direccion de inicio en donde grabar los datos leidos
static unsigned char 	Delimiter;

State
	Resending[],
	Saving[];
//------------------------------------------------------------------
State** Data_Serial_Rx		(void)	{return &Data_Serial_Rx_Sm;}
void 	Init_Serial_Rx		(void)					//inicia las maquinas asociadas al modulo serial
{
 Set_State(Resending,Data_Serial_Rx());					//la maquina de recepcion de datos lista para recibir datos...
 Init_Serial_Phisical();						//inicializa los registros para habilitear el hardware...
}
//----------------------------------------------------------------------------------------------
unsigned char  	Serial_Rx_As_Char	(unsigned int Pos)	{return *(unsigned char*)(Rx_Buffer+Pos);}
unsigned char* 	Serial_Rx_As_PChar	(unsigned int Pos)	{return (unsigned char*) (Rx_Buffer+Pos);}
unsigned int   	Serial_Rx_As_Int	(unsigned int Pos)	{return *(unsigned int*) (Rx_Buffer+Pos);}
unsigned int*  	Serial_Rx_As_PInt	(unsigned int Pos)	{return (unsigned int*)  (Rx_Buffer+Pos);}
unsigned long 	Serial_Rx_As_Long	(unsigned int Pos)	{return *(unsigned long*)(Rx_Buffer+Pos);}
unsigned long* 	Serial_Rx_As_PLong	(unsigned int Pos)	{return (unsigned long*) (Rx_Buffer+Pos);}
unsigned char* 	Serial_Rx_Loaded_Char	(void)			{return Rx_Buffer+0;}
unsigned char* 	Serial_Rx_Loaded_Int	(void)			{return Rx_Buffer+3;}
unsigned char* 	Serial_Rx_Loaded_Long	(void)			{return Rx_Buffer+8;}
//------------------------------------------------------------------------------------------
void 		Save_Char4Serial 		(void)			{Save4Serial2Rx_Buffer	(3, 		0,SERIAL_RX_TOUT);}
void 		Save_Int4Serial 		(void)			{Save4Serial2Rx_Buffer	(5, 		3,SERIAL_RX_TOUT);}
void 		Save_Long4Serial 		(void)			{Save4Serial2Rx_Buffer	(8, 		8,SERIAL_RX_TOUT);}
void 		Send_Loaded_Char2Serial		(void)			{Send_NVData2Serial 	(3,Rx_Buffer+ 	0);}
void 		Send_Loaded_Int2Serial		(void)			{Send_NVData2Serial 	(5,Rx_Buffer+ 	3);}
void 		Send_Loaded_Long2Serial		(void)			{Send_NVData2Serial 	(8,Rx_Buffer+ 	8);}
unsigned char 	Loaded_Binary_Char4Serial	(void)			{return  Dec_Bcd2Char(Serial_Rx_Loaded_Char());}
//------------------------------------------------------------------------------------------
void Save_Data2Buffer(void)	
{
 unsigned char Event=Actual_Event();
 *Point2Read++=Event;										//se guarda el dato arrivado a la vez que se incrementa el puntero para la proxima...
 if(!--Bytes2Read || (Delimiter && Event==Delimiter)) Atomic_Insert_Event(Resending_Event,Data_Serial_Rx());	//se decrementa el numero de bytes que quedan y si es cero...se fuerza el cambio de estados de la mauqina....
}
void Resend_Data2Parser(void)	
{
	Atomic_Insert_Event(Actual_Event(),Serial_Session());
}	//reenvia los datos al parser....
//--------------------------------------------------------------------------------
void Save4Serial(unsigned char Length,unsigned char *Data,unsigned char Time_Out,unsigned char Del)			//lee datos del puerto y los almacena en el lugar indicado
{
 App_Sm=Actual_Sm();										//recuerdo del solicitante....
 if(Time_Out) New_None_Periodic_Schedule(Time_Out,Time_Out_Receiving_Event,Data_Serial_Rx());	//avisa si no llega algo en un rato....
 Point2Read=Data;										//graba el comienzo de los datos....
 Bytes2Read=Length; 
 if(Bytes2Read>0) Atomic_Insert_Event(Save_Data_Event,Data_Serial_Rx());			//Avisa con INSERT que cambie a grabacino si length es mayor a cero...
 Delimiter=Del;
}
void Save4Serial2Rx_Buffer 			(unsigned char Length,unsigned char Pos,unsigned char Time_Out)				{Save4Serial(Length,Serial_Rx_As_PChar(Pos),Time_Out,0);}
void Save4Serial2Rx_Buffer_Until_Delimiter 	(unsigned char Length,unsigned char Pos,unsigned char Time_Out,unsigned char Delimiter)	{Save4Serial(Length,Serial_Rx_As_PChar(Pos),Time_Out,Delimiter);}
void Save4Serial2Rx_Buffer_Until_Enter 		(unsigned char Length)									{Save4Serial2Rx_Buffer_Until_Delimiter(Length,0,SERIAL_RX_TOUT,'\r');}
void Save4Serial_Until_Enter 			(unsigned char Length,unsigned char* Data)						{Save4Serial(Length,Data,SERIAL_RX_TOUT,'\r');}
//-------------------------------------------------------------------------
void Send_Time_Out2Application(void)	
{
// Send_NVDebug_Serial_Rx_Data2Serial(8,"TimeOut\n");
 Atomic_Send_Event(Time_Out_Receiving_Event,App_Sm);
}
void Send_End2Application(void)	
{
// Free_Schedule(Time_Out_Receiving_Event,Data_Serial_Rx());
 Atomic_Send_Event(End_Receiving_Event,App_Sm);
}
//------------------------------------------------------------------
State Resending[] RODATA =
{
{Save_Data_Event		,Rien				,Saving},
{ANY_Event			,Resend_Data2Parser		,Resending},
};
State Saving[] RODATA =
{
{Resending_Event		,Send_End2Application		,Resending},
{Time_Out_Receiving_Event	,Send_Time_Out2Application	,Resending},
{ANY_Event			,Save_Data2Buffer		,Saving},
};
//------------------------------------------------------------------

