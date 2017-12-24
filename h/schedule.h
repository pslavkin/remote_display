#ifndef SCHEDULE
#define SCHEDULE
#include <state_machine.h>
//------------------------------------------------------------------------
#define MAX_SCHEDULE_INDEX 		15    	//define el maximo numero de solicitantes de tiem+outs concurrentes...

enum Schedule_Events 
       	{
	 Timeout1Sec_Event	=0x5000,
	 Timeout1Min_Event	=0x5001
	};
//------------------------------------------------------------------------
extern void Init_Schedule				(void);
extern void Schedule					(void);
extern void Update_Schedule				(unsigned int Time_Out,unsigned int Event, State** Machine);
//------------------------------------------------------------------------
extern void		Free_Schedule				(unsigned int Event, State** Machine);
extern void		Free_All_Schedule			( State** Machine);
extern void		Pause_Schedule				(unsigned int Event, State** Machine);
extern void		Resume_Schedule				(unsigned int Event, State** Machine);
extern void		New_Periodic_Schedule			(unsigned int Time_Out,unsigned int Event, State** Machine);
extern void		New_None_Periodic_Schedule		(unsigned int Time_Out,unsigned int Event, State** Machine);
extern unsigned int 	Read_Schedule				(unsigned int Event, State** Machine);
//------------------------------------------------------------------------
extern void 		New_None_Periodic_Func_Schedule			(unsigned int Time_Out,void (*Func)(void));
extern void 		New_Periodic_Func_Schedule			(unsigned int Time_Out,void (*Func)(void));
extern void 		Pause_Func_Schedule				(void (*Func)(void));
extern void 		Resume_Func_Schedule				(void (*Func)(void));
extern void 		Free_Func_Schedule				(void (*Func)(void));
extern void 		Update_None_Periodic_Func_Schedule		(unsigned int Time_Out,void (*Func)(void));
extern void 		Update_Periodic_Func_Schedule			(unsigned int Time_Out,void (*Func)(void));
extern void 		Update_Or_New_None_Periodic_Func_Schedule 	(unsigned int Time_Out,void (*Func)(void));
extern unsigned int 	Read_Func_Schedule				(void (*Func)(void)); 
//------------------------------------------------------------------------
extern void Periodic1Sec4Sm				(State** Machine);
extern void None_Periodic1Sec				(void);
extern void Free_Wait1Sec				(void);
//------------------------------------------------------------------------
extern void None_Periodic1Min				(void);
extern void Free_Wait1Min				(void);
//------------------------------------------------------------------------


#endif

