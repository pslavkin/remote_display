#ifndef LOG 
#define LOG 

#define LOG_LINES  5
#define LOG_LENGTH 10
struct Log_Table
{
   char Line[LOG_LINES][LOG_LENGTH];
};

extern void                Add_Log       ( void                      );
extern void                Del_Log       ( void                      );
extern void                Init_Log      ( void                      );
extern void                Copy_Pass2Log ( char* Pass, bool Accepted );
extern struct Log_Table*   Read_RLog     ( void                      );
extern void                Print_RLog    ( void                      );
//------------------------------------------------------
#endif
