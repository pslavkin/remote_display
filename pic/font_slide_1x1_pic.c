#include "state_machine.h"
#include "events.h"
#include "display_pics.h"
#include "display_phisical.h"
#include "display_layers.h"
#include "schedule.h"
#include "serial_tx.h"
#include "serial_session.h"
#include "string.h"
#include "type_conversion.h"
#include "buttons.h"
#include "everythings.h"


unsigned char Customs_Chars_Font_Slide_1x1[][8] RODATA=	{
	{0x00,0x00,0x1F,0x1F,0x1F,0x1F,0x00,0x00},
	{0x18,0x1C,0x1E,0x1F,0x1F,0x1E,0x1C,0x18},
};

void Init_Font_Slide_1x1		(void) 	
{
	unsigned char i;
	for (i=0;i<sizeof(Customs_Chars_Font_Slide_1x1)/8;i++) 
		Write_Custom_Char(7-i,Customs_Chars_Font_Slide_1x1[i]); //de atras para adelante...
}
//------------------------------------------------------
