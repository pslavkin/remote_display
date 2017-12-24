#ifndef BUTTONS
#define BUTTONS
//---------------------------------------------------
#define INIT_BUTTONS()			INIT_ROTARY_BUTTON()
#define READ_ALL_BUTTONS()		1 //READ_ROTARY_BUTTON() 
#define HOLD_DELAY			4
//-------------------------------------------------
enum Buttons_Event_Code
{
				Button_None_Event		=0x0020,	//estos salen del hard
				Button_Rotary_Event		=0x0000,

				Any_Button_Event		=0xB000,	//estos son inventados.
				Invalid_Button_Event		=0xB001,
				Wait_Any_Button_Release_Event   =0xB002,
				Button_Rotary_Hold_Event	=0xB003,

				Button_Rotary_Code		=0x04,	//estos son inventados.
				Invalid_Button_Code		=0x05,	//estos son inventados.
				Any_Button_Code			=0x06	//estos son inventados.
};
//-------------------------------------------------
extern void 		Init_Buttons	(void);
extern State** 		Buttons		(void);
extern void 		Buttons_Rti	(void);
//---------------------------------------------------

#endif
