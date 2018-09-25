#ifndef BKGD
#define BKGD

extern void                Add_Bkgd_Black      ( void );
extern void                Del_Bkgd_Black      ( void );
extern struct Struct_Pic*  Read_Bkgd_Black_Pic ( void );
extern void                Add_Bkgd_Red        ( void );
extern void                Del_Bkgd_Red        ( void );
extern void                Add_Bkgd_Green      ( void );
extern void                Del_Bkgd_Green      ( void );
extern void                Add_Bkgd_Blue       ( void );
extern void                Del_Bkgd_Blue       ( void );
extern void                Add_Bkgd_Yellow     ( void );
extern void                Del_Bkgd_Yellow     ( void );
extern uint16_t *Line_Green_Data[ ];
extern uint16_t *Line_Blue_Data [ ];
extern uint16_t *Line_White_Data[ ];
//------------------------------------------------------
#endif
