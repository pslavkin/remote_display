//se definen constantes para habilitar o deshabilitar ciertas partes del codigo encapsuladas con #ifdef

#define PICS_ENABLED
#define PICS_ENABLED2


//#define DEBUG_ONE_WIRE_TRANSPORT
//#define DEBUG_ONE_WIRE_NETWORK		
#define DEBUG_STATE_MACHINE
//#define DEBUG_SERIAL_RX
//#define DEBUG_SERIAL_SESSION2
//#define DEBUG_EVENTS
//#define DEBUG_EVERYTHINGS
//#define DEBUG_BUTTONS
//#define DEBUG_ROTARY
//#define DEBUG_RELAY
//#define DEBUG_TPM
//#define DEBUG_FLASH_PHISICAL
//#define DEBUG_SCHEDULE
//#define DEBUG_DISPLAY_SESSION
#define DEBUG_DISPLAY_LAYERS
//#define DEBUG_ESTIM_PHISICAL
#define DEBUG_POWER_PIC_PHISICAL
#define DEBUG_WELCOME_PIC_PHISICAL

//-------------------------------------------------------------------------------
#ifdef DEBUG_POWER_PIC_PHISICAL
#define Send_NVDebug_Power_Pic_Data2Serial(Length,Data)		(Send_NVData2Serial(Length,(unsigned char*)Data))
#define Send_VDebug_Power_Pic_Data2Serial(Length,Data)		(Send_VData2Serial(Length ,(unsigned char*)Data))
#define Send_NVDebug_Power_Pic_Int_NLine2Serial(Data)		(Send_Int_NLine2Serial(Data))
#define Send_NVDebug_Power_Pic_Char_NLine2Serial(Data)		(Send_Char_NLine2Serial(Data))
#else
#define Send_NVDebug_Power_Pic_Data2Serial(Length,Data)		
#define Send_VDebug_Power_Pic_Data2Serial(Length,Data)		
#define Send_NVDebug_Power_Pic_Int_NLine2Serial(Data)		
#define Send_NVDebug_Power_Pic_Char_NLine2Serial(Data)		
#endif	

#ifdef DEBUG_WELCOME_PIC_PHISICAL
#define Send_NVDebug_Welcome_Pic_Data2Serial(Length,Data)	(Send_NVData2Serial(Length,(unsigned char*)Data))
#define Send_VDebug_Welcome_Pic_Data2Serial(Length,Data)	(Send_VData2Serial(Length ,(unsigned char*)Data))
#define Send_NVDebug_Welcome_Pic_Int_NLine2Serial(Data)		(Send_Int_NLine2Serial(Data))
#define Send_NVDebug_Welcome_Pic_Char_NLine2Serial(Data)	(Send_Char_NLine2Serial(Data))
#else
#define Send_NVDebug_Welcome_Pic_Data2Serial(Length,Data)		
#define Send_VDebug_Welcome_Pic_Data2Serial(Length,Data)		
#define Send_NVDebug_Welcome_Pic_Int_NLine2Serial(Data)		
#define Send_NVDebug_Welcome_Pic_Char_NLine2Serial(Data)		
#endif	

//DEBUG_ESTIM_PHISICAL
#ifdef DEBUG_ESTIM_PHISICAL
#define Send_NVDebug_Estim_Phisical_Data2Serial(Length,Data)	Send_NVData2Serial(Length,(unsigned char*)Data)
#define Send_VDebug_Estim_Phisical_Data2Serial(Length,Data)	Send_VData2Serial(Length,(unsigned char*)Data)
#define Send_NVDebug_Estim_Phisical_Int_NLine2Serial(Data)	Send_Int_NLine2Serial(Data)
#define Send_NVDebug_Estim_Phisical_Char_NLine2Serial(Data)	Send_Char_NLine2Serial(Data)
#else	
#define Send_NVDebug_Estim_Phisical_Data2Serial(Length,Data)	
#define Send_VDebug_Estim_Phisical_Data2Serial(Length,Data)	
#define Send_NVDebug_Estim_Phisical_Int_NLine2Serial(Data) 	
#define Send_NVDebug_Estim_Phisical_Char_NLine2Serial(Data)	
#endif

#ifdef DEBUG_DISPLAY_LAYERS
#define Send_NVDebug_Display_Layers_Data2Serial(Length,Data)	(Send_NVData2Serial(Length,(unsigned char*)Data))
#define Send_VDebug_Display_Layers_Data2Serial(Length,Data)	(Send_VData2Serial(Length ,(unsigned char*)Data))
#define Send_NVDebug_Display_Layers_Int_NLine2Serial(Data)	Send_Int_NLine2Serial(Data)
#define Send_NVDebug_Display_Layers_Char_NLine2Serial(Data)	Send_Char_NLine2Serial(Data)
#else
#define Send_NVDebug_Display_Layers_Data2Serial(Length,Data)	
#define Send_VDebug_Display_Layers_Data2Serial(Length,Data)	
#define Send_NVDebug_Display_Layers_Int_NLine2Serial(Data)	
#define Send_NVDebug_Display_Layers_Char_NLine2Serial(Data)	
#endif	

#ifdef DEBUG_DISPLAY_SESSION
#define Send_NVDebug_Display_Session_Data2Serial(Length,Data)	(Send_NVData2Serial(Length,(unsigned char*)Data))
#define Send_VDebug_Display_Session_Data2Serial(Length,Data)	(Send_VData2Serial(Length ,(unsigned char*)Data))
#else
#define Send_NVDebug_Display_Session_Data2Serial(Length,Data)	
#define Send_VDebug_Display_Session_Data2Serial(Length,Data)	
#endif	

#ifdef DEBUG_SERIAL_RX2
#define Send_NVDebug_Serial_Rx_Data2Serial(Length,Data)	(Send_NVData2Serial(Length,(unsigned char*)Data))
#define Send_VDebug_Serial_Rx_Data2Serial(Length,Data)	(Send_VData2Serial(Length ,(unsigned char*)Data))
#else
#define Send_NVDebug_Serial_Rx_Data2Serial(Length,Data)	
#define Send_VDebug_Serial_Rx_Data2Serial(Length,Data)	
#endif	

#ifdef DEBUG_SERIAL_SESSION2
#define Send_NVDebug_Serial_Session_Data2Serial(Length,Data)	(Send_NVData2Serial(Length,(unsigned char*)Data))
#define Send_VDebug_Serial_Session_Data2Serial(Length,Data)	(Send_VData2Serial(Length ,(unsigned char*)Data))
#else
#define Send_NVDebug_Serial_Session_Data2Serial(Length,Data)	
#define Send_VDebug_Serial_Session_Data2Serial(Length,Data)	
#endif	

#ifdef DEBUG_EVENTS
#define Send_NVDebug_Events_Data2Serial(Length,Data)	(Send_NVData2Serial(Length,(unsigned char*)Data))
#define Send_VDebug_Events_Data2Serial(Length,Data)	(Send_VData2Serial(Length ,(unsigned char*)Data))
#else
#define Send_NVDebug_Events_Data2Serial(Length,Data)	
#define Send_VDebug_Events_Data2Serial(Length,Data)	
#endif

#ifdef DEBUG_STATE_MACHINE
#define Send_NVDebug_State_Machine_Data2Serial(Length,Data)		(Send_NVData2Serial(Length,(unsigned char*)Data))
#else                                                                                              
#define Send_NVDebug_State_Machine_Data2Serial(Length,Data)		
#endif

#ifdef DEBUG_EVERYTHINGS
#define Send_NVDebug_Everythings_Data2Serial(Length,Data)	(Send_NVData2Serial(Length,(unsigned char*)Data))
#define Send_VDebug_Everythings_Data2Serial(Length,Data)	(Send_NVData2Serial(Length,(unsigned char*)Data))
#else
#define Send_NVDebug_Everythings_Data2Serial(Length,Data)	(Length+Data)
#define Send_VDebug_Everythings_Data2Serial(Length,Data)	(Length+Data)
#endif	


#ifdef DEBUG_BUTTONS
#define Send_NVDebug_Buttons_Data2Serial(Length,Data)	(Send_NVData2Serial(Length,(unsigned char*)Data))
#define Send_VDebug_Buttons_Data2Serial(Length,Data)	(Send_NVData2Serial(Length,(unsigned char*)Data))
#else
#define Send_NVDebug_Buttons_Data2Serial(Length,Data)	
#define Send_VDebug_Buttons_Data2Serial(Length,Data)	
#endif	

#ifdef DEBUG_ROTARY
#define Send_NVDebug_Rotary_Data2Serial(Length,Data)	(Send_NVData2Serial(Length,(unsigned char*)Data))
#define Send_VDebug_Rotary_Data2Serial(Length,Data)	(Send_NVData2Serial(Length,(unsigned char*)Data))
#else
#define Send_NVDebug_Rotary_Data2Serial(Length,Data)
#define Send_VDebug_Rotary_Data2Serial(Length,Data)
#endif	

#ifdef DEBUG_RELAY
#define Send_NVDebug_Relay_Data2Serial(Length,Data)	(Send_NVData2Serial(Length,(unsigned char*)Data))
#define Send_VDebug_Relay_Data2Serial(Length,Data)	(Send_NVData2Serial(Length,(unsigned char*)Data))
#else
#define Send_NVDebug_Relay_Data2Serial(Length,Data)	
#define Send_VDebug_Relay_Data2Serial(Length,Data)	
#endif	

#ifdef DEBUG_TPM
#define Send_NVDebug_Tpm_Data2Serial(Length,Data)	(Send_NVData2Serial(Length,(unsigned char*)Data))
#define Send_VDebug_Tpm_Data2Serial(Length,Data)	(Send_NVData2Serial(Length,(unsigned char*)Data))
#else
#define Send_NVDebug_Tpm_Data2Serial(Length,Data)	
#define Send_VDebug_Tpm_Data2Serial(Length,Data)	
#endif	

#ifdef DEBUG_FLASH_PHISICAL
#define Send_NVDebug_Flash_Phisical_Data2Serial(Length,Data)	(Send_NVData2Serial(Length,(unsigned char*)Data))
#define Send_VDebug_Flash_Phisical_Data2Serial(Length,Data)	(Send_NVData2Serial(Length,(unsigned char*)Data))
#else
#define Send_NVDebug_Flash_Phisical_Data2Serial(Length,Data)	
#define Send_VDebug_Flash_Phisical_Data2Serial(Length,Data)	
#endif	


#ifdef  DEBUG_SCHEDULE
#define Send_NVDebug_Schedule_Data2Serial(Length,Data)	(Send_NVData2Serial(Length,(unsigned char*)Data))
#define Send_VDebug_Schedule_Data2Serial(Length,Data)	(Send_NVData2Serial(Length,(unsigned char*)Data))
#else
#define Send_NVDebug_Schedule_Data2Serial(Length,Data)	
#define Send_VDebug_Schedule_Data2Serial(Length,Data)	
#endif	


