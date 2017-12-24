#ifndef SETTING_BANK
#define SETTING_BANK
//------------------------------------------------------
extern struct Struct_Pic 		Setting_Bank_Pic;
extern void 				Add_Setting_Bank		(void);
extern void 				Del_Setting_Bank		(void);
extern void 				Disable_Setting_Bank		(void);	
extern void 				Enable_Setting_Bank		(void);	
extern void 				Init_Custom_Chars_Programs	(void);
extern void 				Init_Custom_Chars_Memory	(void);

extern void 				Select_Programs			(void);
extern void 				Select_Change_Id		(void);
extern void 				Select_A			(void);
extern void 				Select_B			(void);

extern void 				Setting_Programs_Up		(void);
extern void 				Setting_Programs_Down		(void);
extern void 				Change_Id_Up			(void);
extern void 				Change_Id_Down			(void);
extern void 				Memory_Up			(void);
extern void 				Memory_Down			(void);
extern void 				Select_Memory			(void);
extern void 				Select_A_Up			(void);
extern void 				Select_A_Down			(void);
extern void 				Select_B_Up			(void);
extern void 				Select_B_Down			(void);
//------------------------------------------------------
#endif
