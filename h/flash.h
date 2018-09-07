#ifndef  FLASH
#define  FLASH

enum FLASH_COMMANDS {
   FLASH_SECTOR_ERASE_CMD = 0x09, //sector de 0x1000 (4096) bytes
   FLASH_SECTOR_SAVE_CMD  = 0x0B, //graba de a 16bytes minimo
   FLASH_FLEX_AS_RAM_CMD  = 0x81 //la flex ram la usa como buffer para luego grabarla a la flash
};

extern void Init_Flash            ( void                          );
extern void FErase                ( void* Address                 );
extern void Save_Table_Log        ( void                          );
extern void Save_FlexRam2Flash    ( void* Address,uint32_t Length );
extern void Invalidate_Cache      ( void                          );
extern void FlexRam_As_Ram        ( void                          );
extern void Print_FLog            ( void                          );
#endif

