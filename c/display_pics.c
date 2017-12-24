#include "mke18f16.h"
#include "everythings.h"
#include "state_machine.h"
#include "events.h"
#include "display_pics.h"
#include "buttons.h"
#include "type_conversion.h"

//-------------------------------------------------------------------------------------
struct Struct_Pic_Events Rien_Events[] RODATA=		
{{
	Invalid_Button_Code,{Rien,Rien,Rien}
}};
//--------------------------PICS---------------------------------------------------------
void Init_Display_Pics(void) 	
{
	Add_Welcome();
}

