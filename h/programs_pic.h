#ifndef PROGRAMS
#define PROGRAMS


#define MAX_CHANNEL_SEQUENCE 7
//------------------------------------------------------
extern void 				Init_Programs				(void);
extern struct Struct_Pic 		Programs_Pic;
extern void 				Add_Programs				(void);
extern void 				Del_Programs				(void);
extern void 				Select_Program				(void);
extern void 				Select_Eeprom_Program			(void);  
//------------------------------------------------------
extern void				Reset_Sequence_Index			(void);
extern unsigned char 			Actual_Sequence_Data			(void);
extern unsigned char 			Next_Sequence_Channel			(void);
extern void 				Save_Banks2Eeprom_Program		(unsigned char Eeprom_Program_Index);
//------------------------------------------------------
extern void 				V1Scroll2Up_And_Dec_Setting_Param	(void);
extern void 				V1Scroll2Down_And_Inc_Setting_Param	(void);
//------------------------------------------------------
extern void 				Print_Eeprom_Programs			(void);
//------------------------------------------------------
#endif
