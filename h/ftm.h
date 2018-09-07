#ifndef  FTM
#define  FTM

//-----------------------------------------------------------
extern void    Init_Ftm3C0         ( void                     );

extern void    Init_Ftm0C0         ( void                     );
extern void    Set_Bligth          ( uint8_t Percent          );
extern void    Set2Accelerate_Ftm0 ( uint8_t Ch ,uint16_t Pwm );
extern void    Set_Bligth          ( uint8_t Percent          );
extern void    Inc_Bligth          ( void                     );
extern void    Dec_Bligth          ( void                     );
extern void    Inc10_Bligth        ( void                     );
extern void    Dec10_Bligth        ( void                     );
extern void    Accelerate_Ftm0_Log ( unsigned char Ch         );
extern void    Accelerate_All_Ftm0 ( void                     );
extern void    Set_Bligth          ( uint8_t Percent          );
extern void    Print_BLigth        ( void                     );
//----------------------------------------------------
#endif

