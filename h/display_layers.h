#ifndef DISPLAY_LAYERS
#define DISPLAY_LAYERS

#include "state_machine.h"
//---------------------------------------------------------
#define MAX_PICS  25 //
//---------------------------------------------------------
enum Display_Layers_Event_Code
 {
   //Info_Modified_Event      = 0xB001,
   Structure_Modified_Event = 0xB002,
   Clear_Lcd_Event          = 0xB003,
   All_Updated_Event        = 0xB004,
   All_Displayed_Event      = 0xB005,
   Next_Layer_Event         = 0xB006,
   Next_Sub_Pic_Event       = 0xB007
 };

//---------------------------------------------------------
extern void       Init_Display_Layers ( void );
extern State**    Display_Layers      ( void );
extern void       Display_Layers_Rti  ( void );
//---------------------------------------------------------
struct Struct_Pic_Layer
{
 struct Struct_Pic   *Pic        ;
 uint16_t             Actual_Blink;
 uint16_t             Blink_Period;
 unsigned char       Blink_State ;
};

//---------------------------------------------------------
extern void          Print_Layers_Used        ( void                                                 );
// ---------------   ------------------------------------------
extern void          Update_Blink_Pic         ( struct Struct_Pic* Pic                               );
extern void          Do_Blink_Pic             ( struct Struct_Pic* Pic,unsigned int Period           );
extern void          Dont_Blink_Pic           ( struct Struct_Pic* Pic                               );
extern void          Do_Blink_Only_Pic        ( struct Struct_Pic* Pic,unsigned int Period           );
extern void          Dont_Blink_Any_Pic       ( void                                                 );
// ---------------   ------------------------------------------
extern void          Block_Pic_Events         ( struct Struct_Pic* Pic                               );
extern void          Unblock_Pic_Events       ( struct Struct_Pic* Pic,unsigned char Ecount          );
// ---------------------------------------------------------
extern void          Move_Pic2Higth           ( unsigned char Bits, struct Struct_Pic* Pic           );
extern void          Move_Pic2Low             ( unsigned char Bits, struct Struct_Pic* Pic           );
extern void          Move_Pic2Rigth           ( unsigned char Bits, struct Struct_Pic* Pic           );
extern void          Move_Pic2Left            ( unsigned char Bits, struct Struct_Pic* Pic           );
extern void          Move_Pic2Top             ( struct Struct_Pic* Pic                               );
extern void          Move_Pic2Bottom          ( struct Struct_Pic* Pic                               );
extern void          Add_Pic_On_Top           ( struct Struct_Pic* Pic                               );
extern void          Add_Pic_On_Bottom        ( struct Struct_Pic* Pic                               );
extern void          Add_Pic_On_Layer         ( struct Struct_Pic* Pic,unsigned char Layer           );
extern void          Del_Pic                  ( struct Struct_Pic* Pic                               );
extern void          Del_All_Layers           ( void                                                 );
extern bool          Find_Event_Handler       ( uint8_t Button,uint16_t X,uint16_t Y,uint8_t Handler );
extern void          Layer_Clr_Lcd            ( void                                                 );
extern void          Layer_Structure_Modified ( void                                                 );
extern void          Set_Watermark_Layer      ( void                                                 );
extern void          Reset_Watermark_Layer    ( void                                                 );
extern unsigned char Layer_Used               ( void                                                 );
extern void          Print_Layer_Used         ( void                                                 );
//---------------------------------------------------------
extern void       Move_Pic_Bits2Higth  ( unsigned char Bits  ,struct Struct_Pic* Pic );
extern void       Move_Pic_Bits2Low    ( unsigned char Bits  ,struct Struct_Pic* Pic );
extern void       Move_Pic_Bytes2Rigth ( unsigned char Bytes ,struct Struct_Pic* Pic );
extern void       Move_Pic_Bytes2Left  ( unsigned char Bytes ,struct Struct_Pic* Pic );
extern void       Move_Pic_Bits2Rigth  ( unsigned char Bits  ,struct Struct_Pic* Pic );
extern void       Move_Pic_Bits2Left   ( unsigned char Bits  ,struct Struct_Pic* Pic );
//---------------------------------------------------------
extern void             All_Displayed         ( void );
extern unsigned char    Disp_Bank_Updated     ( void );
// ---------------------------------------------------------
extern void             Send_Next_Layer_Event ( void );

#endif

