
/* Take into account assert bug in binutils-2.15*/
#if (USE_BINUTILS_2_19 == 1)
#define LD_ASSERT(condition, small, full)   ASSERT(condition, full)
#else
#define LD_ASSERT(condition, small, full)   ASSERT(condition, small)
#endif

/*Mandatory to have the BB side irq handler linked */
ENTRY (user_Main)
/*ENTRY (aiuser_init)*/

SECTIONS
{
    /***************************************
    **               ROM                  **
    ***************************************/
    /* flash start address*/
    . = USER_ROM_BASE;
    __rom_start = .;
    
    /* RO */
    .section_init . : AT (ADDR(.section_init))
    {
        KEEP(*(.section_init))
        . = ALIGN(0x10);
    }
    .cust_rotext . : AT (ADDR(.cust_rotext))
    {
        *(.text)
        *(.text.*)
        *(.stub .gnu.linkonce.t.*)
        . = ALIGN(0x20);
    }

    .cust_rodata . : AT (ADDR(.cust_rodata))
    {
        *(.rdata)
        *(.rodata)
        *(.rodata.*)
        *(.rodata.*_png)
        *(.gnu.linkonce.r.*) 
        *(.rodata1)
        *(.roresdata)
        *(.robsdata)
        . = ALIGN(0x20);
    } 
	__user_rw_lma = .;
    __rom_size = __user_rw_lma - __rom_start;
    /* RAM */
    . = USER_RAM_BASE;
    /* RW */
    .cust_data . : AT ( __user_rw_lma )
    {
        __user_rw_start = .;
        *(.data)
        *(.data.*)
        *(.gnu.linkonce.d.*)
        *(.sdata .sdata.* .gnu.linkonce.s.*)
        *(.sdata2 .sdata2.* .gnu.linkonce.s2.*)
        . = ALIGN(0x10);
    }
    __user_rw_end = .;
    __user_rw_size = __user_rw_end - __user_rw_start;
    
    __user_bss_start = .;
    /* ZI */
    .cust_bss . : AT (ADDR(.cust_bss))
    {
        *(.sbss .sbss.* .gnu.linkonce.sb.*)
        *(.sbss2 .sbss2.* .gnu.linkonce.sb2.*)
        *(.scommon)
        *(.dynbss)
        *(.bss .bss.* .gnu.linkonce.b.*)
        *(COMMON)
        *(.rambss)
        . = ALIGN(0x10);
    }
    __user_bss_end = .;
    __user_bss_size = __user_bss_end - __user_bss_start;
    #include "discard.ld"
}
