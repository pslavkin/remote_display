#include <schedule.h>
#include <state_machine.h>
#include <events.h>
#include <string.h>

struct Schedule_Event_Machine   	//estructura que define los campos que se usaran para almacenar los tim_outs solicitados...
  {
   unsigned int Fixed_Time_Out;		//este entero se acuerda de lo que le pidieron inicialmente para eventos repetitivos...
   unsigned int Time_Out;          	//un entero para el time_out para poder tener tiempos largos...
   unsigned int Event;             	//el evento que enviara cuando se acabe el tiempo...
   unsigned char Enabled;
   union 
   {
     State      **Machine;
    void (*Func) (void);
   }Sm_Func;
  } Schedule_Event_Machine;

struct Schedule_Event_Machine Schedule_List[MAX_SCHEDULE_INDEX]; 
//----------------------------------------------------------------------
void Init_Schedule(void)
{
 unsigned char i;
 for(i=0;i<MAX_SCHEDULE_INDEX;i++) Schedule_List[i].Sm_Func.Machine=Empty_State_Machine;
}
//----------------------------------------------------------------------
void Schedule(void)  
{
	unsigned char i;
	for(i=0;i<MAX_SCHEDULE_INDEX;i++)    											//para toda la lista...
		if(Schedule_List[i].Sm_Func.Machine!=Empty_State_Machine && Schedule_List[i].Enabled==1) {			//si la entrada no es libre y esta abilitada...
			if(!Schedule_List[i].Time_Out) {									//si el decrementado time_out es cero....
				if(Schedule_List[i].Event!=Invalid_Event) 
					Atomic_Send_Event(Schedule_List[i].Event,Schedule_List[i].Sm_Func.Machine);		//ei el evento es valido manda un mensaje a la maquina de estados que lo solicito...
				else
					Schedule_List[i].Sm_Func.Func();							//si el evento es invalido, entonces es un schedule de funcion y se la ejecuta asi sin mas...
				if(!Schedule_List[i].Fixed_Time_Out) 									//si el tiempo fijo es cer, se trata de no periodico
					Schedule_List[i].Sm_Func.Machine=Empty_State_Machine;						//se libera la entrada....
				else 
					Schedule_List[i].Time_Out=Schedule_List[i].Fixed_Time_Out-1;					//sino se transfiere pero fijo menos uno.. para permitir el minimo de tiempo que es que entre en cada pasada
			}
			else 
				--Schedule_List[i].Time_Out;
	}
	return;
}
//----------------------------------------------------------------------
void Add_Schedule(unsigned char Index,unsigned int Fixed_Time_Out,unsigned int Time_Out,unsigned int Event, State** Machine,void (*Func)(void),unsigned char Sm_Func) 
{
 Schedule_List[Index].Fixed_Time_Out=Fixed_Time_Out;//?--Fixed_Time_Out:Fixed_Time_Out;
 Schedule_List[Index].Time_Out=Time_Out?--Time_Out:Time_Out;
 Schedule_List[Index].Event=Event;
 Schedule_List[Index].Enabled=1;
 if(!Sm_Func) Schedule_List[Index].Sm_Func.Machine=Machine; else Schedule_List[Index].Sm_Func.Func=Func;
}
unsigned char Search_Schedule_Index(void)
{
 unsigned char i;
 for(i=0;i<MAX_SCHEDULE_INDEX && Schedule_List[i].Sm_Func.Machine!=Empty_State_Machine;i++);
 return i;
}
unsigned char Find_Function(void (*Func)(void))	
{
 unsigned char i;
 for(i=0;i<MAX_SCHEDULE_INDEX && Schedule_List[i].Sm_Func.Func!=Func;i++);
 return i;
}
//----------------------------------------------------------------------------------------------------
unsigned char   Find_Schedule_Index                     (unsigned int Event, State** Machine)
{
 unsigned char i;
 for(i=0;i<MAX_SCHEDULE_INDEX && !(Schedule_List[i].Sm_Func.Machine==Machine && Schedule_List[i].Event==Event);i++);
 return i;
}
unsigned char   Find_Func_Schedule_Index                (void (*Func)(void))
{
 unsigned char i;
 for(i=0;i<MAX_SCHEDULE_INDEX && Schedule_List[i].Sm_Func.Func!=Func;i++);      //recorre todas las entradas fijas en b
 return i;                                                                      //si no hubo lugar, lamento mucho devue
}
//----------------------------------------------------------------------------------------------------
unsigned char Find_Event_Machine(unsigned int Event, State** Machine)
{
 unsigned char i;
 for(i=0;i<MAX_SCHEDULE_INDEX && !(Schedule_List[i].Sm_Func.Machine==Machine && Schedule_List[i].Event==Event);i++);
 return i;
}
 //----------------------------------------------------------------------
void Update_Schedule(unsigned int Time_Out,unsigned int Event, State** Machine)
{
 unsigned char i=Find_Event_Machine(Event,Machine);
 if(i<MAX_SCHEDULE_INDEX) Add_Schedule(i,Time_Out,Time_Out,Event,Machine,Rien,0);
}
void Free_Schedule(unsigned int Event, State** Machine)
{
 unsigned char i=Find_Event_Machine(Event,Machine);
 if(i<MAX_SCHEDULE_INDEX) Add_Schedule(i,0,0,0,Empty_State_Machine,Rien,0);
}
void Free_All_Schedule( State** Machine)
{
 unsigned char i;
 for(i=0;i<MAX_SCHEDULE_INDEX;i++)
  if(Schedule_List[i].Sm_Func.Machine==Machine) Add_Schedule(i,0,0,0,Empty_State_Machine,Rien,0);
}
void Pause_Schedule(unsigned int Event, State** Machine)
{
 unsigned char i=Find_Event_Machine(Event,Machine);
 if(i<MAX_SCHEDULE_INDEX) Schedule_List[i].Enabled=0;
}
void Resume_Schedule(unsigned int Event, State** Machine)
{
 unsigned char i=Find_Event_Machine(Event,Machine);
 if(i<MAX_SCHEDULE_INDEX) Schedule_List[i].Enabled=1;
}
void New_Periodic_Schedule(unsigned int Time_Out,unsigned int Event, State** Machine)
{
 unsigned char i=Search_Schedule_Index(); 
 if(i<MAX_SCHEDULE_INDEX) Add_Schedule(i,Time_Out,Time_Out,Event,Machine,Rien,0);
}
void New_None_Periodic_Schedule(unsigned int Time_Out,unsigned int Event, State** Machine)
{
 unsigned char i=Search_Schedule_Index(); 
 if(i<MAX_SCHEDULE_INDEX) Add_Schedule(i,0,Time_Out,Event,Machine,Rien,0);
}
unsigned int Read_Schedule(unsigned int Event, State** Machine)
{
 unsigned char i=Find_Event_Machine(Event,Machine);
 if(i<MAX_SCHEDULE_INDEX) return Schedule_List[i].Time_Out;
 return 0;
}
//----------------------------------------------------------------------
void Free_Func_Schedule(void (*Func)(void))
{
 unsigned char i=Find_Function(Func);
 if(i<MAX_SCHEDULE_INDEX) Add_Schedule(i,0,0,0,Empty_State_Machine,Rien,0);
}
void Update_Periodic_Func_Schedule(unsigned int Time_Out,void (*Func)(void))
{
 unsigned char i=Find_Function(Func);
 if(i<MAX_SCHEDULE_INDEX) Add_Schedule(i,Time_Out,Time_Out,Invalid_Event,Empty_Sm(),Func,1);
}
void Update_None_Periodic_Func_Schedule(unsigned int Time_Out,void (*Func)(void))
{
 unsigned char i=Find_Function(Func);
 if(i<MAX_SCHEDULE_INDEX) Add_Schedule(i,0,Time_Out,Invalid_Event,Empty_Sm(),Func,1);
}
void New_None_Periodic_Func_Schedule(unsigned int Time_Out,void (*Func)(void)) 
{
 unsigned char i=Search_Schedule_Index(); 
 if(i<MAX_SCHEDULE_INDEX) Add_Schedule(i,0,Time_Out,Invalid_Event,Empty_Sm(),Func,1);
}
void New_Periodic_Func_Schedule(unsigned int Time_Out,void (*Func)(void)) 
{
 unsigned char i=Search_Schedule_Index(); 
 if(i<MAX_SCHEDULE_INDEX) Add_Schedule(i,Time_Out,Time_Out,Invalid_Event,Empty_Sm(),Func,1);
}
void Update_Or_New_None_Periodic_Func_Schedule(unsigned int Time_Out,void (*Func)(void)) 
{
 unsigned char i=Find_Function(Func);
 if(i>=MAX_SCHEDULE_INDEX) i=Search_Schedule_Index();
 if(i<MAX_SCHEDULE_INDEX) Add_Schedule(i,0,Time_Out,Invalid_Event,Empty_Sm(),Func,1);
}
unsigned int Read_Func_Schedule(void (*Func)(void))         
{
 unsigned char i=Find_Func_Schedule_Index(Func);
 if(i<MAX_SCHEDULE_INDEX) return Schedule_List[i].Time_Out;
 return 0;
}
void Pause_Func_Schedule	(void (*Func)(void))	{Pause_Schedule (Invalid_Event,(State **)Func);}
void Resume_Func_Schedule	(void (*Func)(void))	{Resume_Schedule(Invalid_Event,(State **)Func);}
//----------------------------------------------------------------------
void Periodic1Sec4Sm		(State** Machine)	{New_Periodic_Schedule(20,Timeout1Sec_Event,Machine);}
void None_Periodic1Sec		(void)			{New_None_Periodic_Schedule(20,Timeout1Sec_Event,Actual_Sm());}
void Free_Wait1Sec		(void)			{Free_Schedule(Timeout1Sec_Event,Actual_Sm());}
//----------------------------------------------------------------------
void None_Periodic1Min		(void)			{New_None_Periodic_Schedule(600,Timeout1Min_Event,Actual_Sm());}
void Free_Wait1Min		(void)			{Free_Schedule(Timeout1Min_Event,Actual_Sm());}

