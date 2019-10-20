extern unsigned int _DATA_ROM_START;
extern unsigned int _DATA_RAM_START;
extern unsigned int _DATA_RAM_END;
extern unsigned int _BSS_START;
extern unsigned int _BSS_END;
extern unsigned int _HEAP_START;
extern unsigned int _HEAP_END;
extern unsigned int _HEAP_SIZE;
extern unsigned int _STACK_SIZE;
extern unsigned int _STACK_BEGIN;
extern unsigned int _STACK_END;

extern void main();

void Reset_Handler()
{
    /* Copy data belonging to the `.data` section from its
     * load time position on flash (ROM) to its run time position
     * in SRAM.
     */
    unsigned int * data_rom_start_p = &_DATA_ROM_START;
    unsigned int * data_ram_start_p = &_DATA_RAM_START;
    unsigned int * data_ram_end_p = &_DATA_RAM_END;

    while(data_ram_start_p != data_ram_end_p)
    {
        *data_ram_start_p = *data_rom_start_p;
        data_ram_start_p++;
        data_rom_start_p++;
    }

    /* Initialize data in the `.bss` section to zeros.
     */
    unsigned int * bss_start_p = &_BSS_START; 
    unsigned int * bss_end_p = &_BSS_END;

    while(bss_start_p != bss_end_p)
    {
        *bss_start_p = 0;
        bss_start_p++;
    }


    /* Call the `main()` function defined in `test_program.c`.
     */
    main();
}

