/* OUTPUT_FORMAT(binary) */
ENTRY(main)

SECTIONS
{
    .text : {
        *(.entry)
        *(.text*)
        *(.rodata*)
        *(.data*)
    }
}