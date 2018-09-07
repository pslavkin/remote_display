#ifndef  FLASHP
#define  FLASHP

#include <stdbool.h>

extern void Init_Flash         ( void                          );
extern void FErase             ( void* Address                 );
extern void Save_Table_Log     ( void                          );
extern void Save_FlexRam2Flash ( void* Address,uint32_t Length );
extern void Invalidate_Cache   ( void                          );
extern void FlexRam_As_Ram     ( void                          );
extern void Print_FLog         ( void                          );
#endif

