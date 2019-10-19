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
    .  = 0x0;         
    
    .vector_table : ALIGN(4)
    {
        KEEP(*(.vector_table))    
    }
    
    .text : ALIGN(4) 
    {
        *(.text)      
    }
    
    .rodata : ALIGN(4)
    {
        *(.rodata)    
    }
    
    _DATA_ROM_START = .;

    .  = 0x20000000;        

    _DATA_RAM_START = .;
    
    .data : AT(_DATA_ROM_START) ALIGN(4) 
    {
        *(.data)      
    }
    
    _DATA_RAM_END = .;

    _BSS_START = .;   
    
    .bss : ALIGN(4)
    {
        *(.bss)       
    }
    
    _BSS_END = .;     
    
    _HEAP_START = .;
    
    .heap : ALIGN(4)
    {
        *(.heap)
    } > RAM
    
    _HEAP_END =.;
    _HEAP_SIZE = _HEAP_END - _HEAP_START;
    
    _STACK_SIZE = 1024;
    _STACK_BEGIN = ORIGIN(RAM) + LENGTH(RAM);
    _STACK_END =  _STACK_BEGIN - _STACK_SIZE;
    
    .stack : ALIGN(4)
    {
        *(.stack)
    } > RAM
}

ENTRY(reset_handler)
