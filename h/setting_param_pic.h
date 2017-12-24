#ifndef SETTING_PARAM
#define SETTING_PARAM
//------------------------------------------------------
struct Struct_Setting_Char_Param
{
 unsigned char Min;
 unsigned char Max;
 unsigned char Inc;
 unsigned char *Value;
 void (*Update)(void);
 void (*Return)(void);
};
struct Struct_Setting_Int_Param
{
 unsigned int Min;
 unsigned int Max;
 unsigned char Inc;
 unsigned int *Value;
 void (*Update)(void);
 void (*Return)(void);
};
//------------------------------------------------------
extern struct Struct_Pic		       Setting_Int_Param_Pic;
extern struct Struct_Pic		       Setting_Char_Param_Pic;
//------------------------------------------------------
extern void 					Setting_Param_Constr		(void);
extern void 					Setting_Param_Destr		(void);
//----------CHAR-----------------------------------------------------------------------------------------
extern void 					Inc_Setting_Char_Param		(void);
extern void 					Dec_Setting_Char_Param		(void);
extern struct Struct_Setting_Char_Param* 	Read_Char_Setting_Params	(void);
extern void 					(*Read_Setting_Char_Params_Func	(void))(void);
extern void 					Pasive_Setting_Char_Param 	(void);
extern void 					Active_Setting_Char_Param 	(void);
extern void 					Setting_Char_Param_Ok		(void);
extern void 					Do_Blink_Setting_Char_Param 	(void);
extern void 					Dont_Blink_Setting_Char_Param	(void);
extern void 					Setting_Char_Param_Update	(void);
extern void 					Set_Setting_Char_Param		(struct Struct_Setting_Char_Param *Params);
extern void 					Update_Setting_Char_Param	(void);
extern void 					Update_Setting_Charx10_Param	(void);
extern void 					Hide_Setting_Char_Param		(void);
extern void 					Unhide_Setting_Char_Param	(void);
//----------INT-----------------------------------------------------------------------------------------
extern void 					Inc_Setting_Int_Param		(void);
extern void 					Dec_Setting_Int_Param		(void);
extern struct Struct_Setting_Int_Param* 	Read_Setting_Int_Params		(void);
extern void 					(*Read_Setting_Int_Params_Func	(void))(void);
extern void 					Pasive_Setting_Int_Param 	(void);
extern void 					Active_Setting_Int_Param 	(void);
extern void 					Setting_Int_Param_Ok		(void);
extern void 					Do_Blink_Setting_Int_Param 	(void);
extern void 					Dont_Blink_Setting_Int_Param	(void);
extern void 					Update_Setting_Int_Param	(void);
extern void 					Set_Setting_Int_Param		(struct Struct_Setting_Int_Param *Params);
extern void 					Setting_Int_Param_Update	(void);
//------------------------------------------------------
extern void					Add_Setting_Char_Param		(void);	
extern void					Del_Setting_Char_Param		(void);	
extern void					Add_Setting_Int_Param		(void);	
extern void					Del_Setting_Int_Param		(void);	
//------------------------------------------------------

#endif
