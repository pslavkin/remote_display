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
// valores por defecto para LCD 240x320 cinta angosta

#define  TP_MAX_Y 224
#define  TP_MIN_Y 32
#define  TP_MAX_X 190
#define  TP_MIN_X 64

#define DELAY_BEFORE_DRAG_FILTER 8
//------------------------------------------------------------------------------
enum Buttons_Event_Code {
   None_Button        = 0x00,
   Any_Button         = 0x80,
   Invalid_Button     = 0x81,
   None_Touched_Event = 0x00,
   Click_Event        = 0x01,
   Drag_Event         = 0x02,
   Released_Event     = 0x03
};

struct TPanel_Struct
   {
    uint16_t X;
    uint16_t Y;
    uint16_t X_Scaled;
    uint16_t Y_Scaled;
    uint8_t Touched;
   };
struct TPanel_Limits_Struct
   {
    uint16_t Max_Y;
    uint16_t Min_Y;
    uint16_t Max_X;
    uint16_t Min_X;
    uint16_t Delta_Y;
    uint16_t Delta_X;
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
void           Print_TPanel_Limits        ( void                                  );
void           Print_TPanel_Raw           ( void                                  );
void           Print_Buttons              ( void                                  );
//------------------------------------------------------------------------------
#endif
