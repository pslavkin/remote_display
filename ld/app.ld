MEMORY
{
  /* Define each memory region */
  PROGRAM_FLASH (rx): ORIGIN = 0x0,          LENGTH = 0x7E000  /* 512K bytes (alias Flash) */
  USER_FLASH (rx):    ORIGIN = 0x7E000,      LENGTH = 0x00400  /* 8k para flash de usuario, para poder ir intercambiando de banco. */
  SRAM_UPPER (rwx):   ORIGIN = 0x20000000,   LENGTH = 0x8000   /* 32K bytes (alias RAM) */
  SRAM_LOWER (rwx):   ORIGIN = 0x1fff8000,   LENGTH = 0x8000   /* 32K bytes (alias RAM2) */
  FLEX_RAM (rwx):     ORIGIN = 0x14000000,   LENGTH = 0x1000   /* 4K bytes (alias RAM3) */
}

  /* Define a symbol for the top of each memory region */
  __base_PROGRAM_FLASH = 0x0;/* PROGRAM_FLASH */
  __base_Flash         = 0x0 ; /* Flash */
  __top_PROGRAM_FLASH  = 0x0 + 0x80000 ; /* 512K bytes */
  __top_Flash          = 0x0 + 0x80000 ; /* 512K bytes */
  __base_SRAM_UPPER    = 0x20000000  ; /* SRAM_UPPER */
  __base_RAM           = 0x20000000 ; /* RAM */
  __top_SRAM_UPPER     = 0x20000000 + 0x8000 ; /* 32K bytes */
  __top_RAM            = 0x20000000 + 0x8000 ; /* 32K bytes */
  __base_SRAM_LOWER    = 0x1fff8000  ; /* SRAM_LOWER */
  __base_RAM2          = 0x1fff8000 ; /* RAM2 */
  __top_SRAM_LOWER     = 0x1fff8000 + 0x8000 ; /* 32K bytes */
  __top_RAM2           = 0x1fff8000 + 0x8000 ; /* 32K bytes */
  __base_FLEX_RAM      = 0x14000000  ; /* FLEX_RAM */
  __base_RAM3          = 0x14000000 ; /* RAM3 */
  __top_FLEX_RAM       = 0x14000000 + 0x1000 ; /* 4K bytes */
  __top_RAM3           = 0x14000000 + 0x1000 ; /* 4K bytes */

ENTRY(ResetISR)

SECTIONS
{
    /* MAIN TEXT SECTION */
    .text : ALIGN(4)
    {
        FILL(0xff)
        __vectors_start__ = ABSOLUTE(.) ;
        KEEP(*(.isr_vector))
        /* Global Section Table */
        . = ALIGN(4) ;
        __section_table_start = .;
        __data_section_table = .;
        LONG(LOADADDR(.data));
        LONG(    ADDR(.data));
        LONG(  SIZEOF(.data));
        LONG(LOADADDR(.data_RAM2));
        LONG(    ADDR(.data_RAM2));
        LONG(  SIZEOF(.data_RAM2));
        LONG(LOADADDR(.data_RAM3));
        LONG(    ADDR(.data_RAM3));
        LONG(  SIZEOF(.data_RAM3));
        __data_section_table_end = .;
        __bss_section_table = .;
        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        LONG(    ADDR(.bss_RAM2));
        LONG(  SIZEOF(.bss_RAM2));
        LONG(    ADDR(.bss_RAM3));
        LONG(  SIZEOF(.bss_RAM3));
        __bss_section_table_end = .;
        __section_table_end = . ;
        /* End of Global Section Table */

        *(.after_vectors*)


   /* Bootloader Configuration Area (BCA) */
        . = 0x3C0 ;
        KEEP(*(.BootloaderConfig))
        /* Kinetis Flash Configuration data */
        . = 0x400 ;
        KEEP(*(.FlashConfig))

    } >PROGRAM_FLASH

    .text : ALIGN(4)
    {
        *(.const_initialized*)
        . = ALIGN(4);
    } > PROGRAM_FLASH
    .text : ALIGN(4)
    {
        *(.text*)
        *(.rodata .rodata.* .constdata .constdata.*)
        . = ALIGN(4);
    } > PROGRAM_FLASH
    /*
     * for exception handling/unwind - some Newlib functions (in common
     * with C++ and STDC++) use this. 
     */
    .ARM.extab : ALIGN(4) 
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > PROGRAM_FLASH
    __exidx_start = .;

    .ARM.exidx : ALIGN(4)
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > PROGRAM_FLASH
    __exidx_end = .;

    _etext = .;

    .uflash : ALIGN(4)
    {
        *(.uflash*)
        . = ALIGN(4);
    } > USER_FLASH
    /* DATA section for SRAM_LOWER */
    .data_RAM2 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM2 = .) ;
        *(.ramfunc.$RAM2)
        *(.ramfunc.$SRAM_LOWER)
        *(.data.$RAM2*)
        *(.data.$SRAM_LOWER*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM2 = .) ;

     } > SRAM_LOWER AT>PROGRAM_FLASH

    /* DATA section for FLEX_RAM */
    .data_RAM3 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM3 = .) ;
        *(.ramfunc.$RAM3)
        *(.ramfunc.$FLEX_RAM)
        *(.data.$RAM3*)
        *(.data.$FLEX_RAM*)
        *(.data.FLEX_RAM*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM3 = .) ;
     } > FLEX_RAM AT>PROGRAM_FLASH

    /* MAIN DATA SECTION */
    .uninit_RESERVED : ALIGN(4)
    {
        KEEP(*(.bss.$RESERVED*))
        . = ALIGN(4) ;
        _end_uninit_RESERVED = .;
    } > SRAM_UPPER
    /* Main DATA section (SRAM_UPPER) */
    .data : ALIGN(4)
    {
       FILL(0xff)
       _data = . ;
       *(vtable)
       *(.ramfunc*)
       *(.data*)
       . = ALIGN(4) ;
       _edata = . ;
    } > SRAM_UPPER AT>PROGRAM_FLASH
    /* BSS section for SRAM_LOWER */
    .bss_RAM2 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM2 = .) ;
       *(.bss.$RAM2*)
       *(.bss.$SRAM_LOWER*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM2 = .) ;
    } > SRAM_LOWER 
    /* BSS section for FLEX_RAM */
    .bss_RAM3 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM3 = .) ;
       *(.bss.$RAM3*)
       *(.bss.$FLEX_RAM*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM3 = .) ;
    } > FLEX_RAM 
    /* MAIN BSS SECTION */
    .bss : ALIGN(4)
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
        _ebss = .;
        PROVIDE(end = .);
    } > SRAM_UPPER
    /* NOINIT section for SRAM_LOWER */
    .noinit_RAM2 (NOLOAD) : ALIGN(4)
    {
       *(.noinit.$RAM2*)
       *(.noinit.$SRAM_LOWER*)
       . = ALIGN(4) ;
    } > SRAM_LOWER 
    /* NOINIT section for FLEX_RAM */
    .noinit_RAM3 (NOLOAD) : ALIGN(4)
    {
       *(.noinit.$RAM3*)
       *(.noinit.$FLEX_RAM*)
       . = ALIGN(4) ;
    } > FLEX_RAM 
    /* DEFAULT NOINIT SECTION */
    .noinit (NOLOAD): ALIGN(4)
    {
        _noinit = .;
        *(.noinit*) 
         . = ALIGN(4) ;
        _end_noinit = .;
    } > SRAM_UPPER

    /* Reserve and place Heap within memory map */
    _HeapSize = 0x800;
    .heap :  ALIGN(4)
    {
        _pvHeapStart = .;
        . += _HeapSize;
        . = ALIGN(4);
        _pvHeapLimit = .;
    } > SRAM_UPPER

     _StackSize = 0x800;
     /* Reserve space in memory for Stack */
    .heap2stackfill  :
    {
        . += _StackSize;
    } > SRAM_UPPER
    /* Locate actual Stack in memory map */
    .stack ORIGIN(SRAM_UPPER) + LENGTH(SRAM_UPPER) - _StackSize - 0:  ALIGN(4)
    {
        _vStackBase = .;
        . = ALIGN(4);
        _vStackTop = . + _StackSize;
    } > SRAM_UPPER



    /* Provide basic symbols giving location and size of main text
     * block, including initial values of RW data sections. Note that
     * these will need extending to give a complete picture with
     * complex images (e.g multiple Flash banks).
     */
    _image_start = LOADADDR(.text);
    _image_end = LOADADDR(.data) + SIZEOF(.data);
    _image_size = _image_end - _image_start;
}
