#ifndef MASK
#define MASK

#include <stdint.h>

extern void 			Update_Mask_Pic		(void);
extern void 			Resize_Mask_Pic		(uint16_t Start_X,uint16_t Width,uint16_t Start_Y,uint16_t Height);
extern struct Struct_Pic* 	Read_Mask_Pic		(void);
extern void 			Add_Mask0		(void);
extern void 			Add_Mask1		(void);
extern void 			Add_Mask2		(void);
extern void 			Add_Mask3		(void);
extern void 			Add_Mask4		(void);
extern void 			Add_Mask5		(void);
extern void 			Add_Mask6		(void);
extern void 			Add_Mask7		(void);
extern void 			Add_Mask8		(void);
extern void 			Add_Mask9		(void);
extern void 			Add_Mask_Enter		(void);
extern void 			Add_Mask_Delete		(void);
extern void 			Del_Mask		(void);
//------------------------------------------------------
#endif
