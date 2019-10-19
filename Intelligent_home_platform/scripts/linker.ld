/* Custom linker script for STM32F103RBT6 (STM32 Nucleo-F103RB) 
 * 
 * 
 */

MEMORY
{
    FLASH (rx) : ORIGIN = 0x0, LENGTH = 0x20000 /* 128K */
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x5000 /* 20K */
}

SECTIONS
{
    .  = 0x0;         /* From 0x00000000 */

    .text : 
    {
        *(vectors)    /* Vector table */
        *(.text)      /* Program code */
    }
    .rodata : 
    {
        *(.rodata)    /* Read only data */
    }
    _DATA_ROM_START = .;

    .  = 0x20000000;  /* From 0x20000000 */      

    _DATA_RAM_START = .;
    .data : AT(_DATA_ROM_START) 
    {
        *(.data)      /* Data memory */
    }
    _DATA_RAM_END = .;

    __bss_start__ = .;   /* Indicates where BSS section starts in RAM */
    .bss :
    {
        *(.bss)       /* Zero-filled run time allocate data memory */
    }
    __bss_end__ = .;     /* Indicates where BSS section ends in RAM */
}

