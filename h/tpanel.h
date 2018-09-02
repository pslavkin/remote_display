#ifndef TPANNEL
#define TPANNEL

/*
                           5v        5v
                           |         |
                           \         |
                           /         |
                           \         /
                           |         \
    PTA0---YU--------------|         /
                        ___|____     |
                        |      |     |
                        |      |     |
    PTA16--XL---AD13----|      |-----|
                        |______|     |
                           |         |
    PTA15--YD---AD12-------|         |
                                     |
    PTA1---XR-------------------------
*/

//---------------------------------------
// bordes para el touch Winstar TS12864ARNB 
// NUEVO con pegapega verde

#define  TP_MAX_Y 224
#define  TP_MIN_Y 32
#define  TP_MAX_X 190
#define  TP_MIN_X 64
//------------------------------------------------------------------------------
struct TPanel_Struct
   {
    unsigned char X;
    unsigned char Y;
    unsigned char X_Scaled;
    unsigned char Y_Scaled;
    unsigned char Button;
    unsigned char Last_Button;
    unsigned char Touched;
   };
struct TPanel_Limits_Struct
   {
    unsigned char Max_Y;
    unsigned char Min_Y;
    unsigned char Max_X;
    unsigned char Min_X;
    unsigned char Delta_Y;
    unsigned char Delta_X;
   };
//------------------------------------------------------------------------------
void           Init_Tpanel ( void                                  );
void           Tpanel_Rti  ( void                                  );
State**        Tpanel      ( void                                  );
//-------------------------------------------------------------------------
unsigned char  Touched              ( void                                  );
unsigned char  TPanel_X             ( void                                  );
unsigned char  TPanel_Y             ( void                                  );
unsigned char  TPanel_Scaled_X      ( void                                  );
unsigned char  TPanel_Scaled_Y      ( void                                  );
//-------------------------------------------------------------------------
unsigned char  Read_TPanel_Limits         ( unsigned char Pos                     );
void           Reset_TPanel_Limits        ( void                                  );
void           Save_TPanel_All_Limits     ( struct TPanel_Limits_Struct* Limits   );
void           Save_TPanel_Limits         ( unsigned char Pos,unsigned char Limit );
void           Save_TPanel_Default_Limits ( void                                  );
void           Print_TPanel_Limits        ( void                                  );
void           Print_TPanel_Raw           ( void                                  );
void           Print_Buttons              ( void                                  );
//------------------------------------------------------------------------------
#endif
