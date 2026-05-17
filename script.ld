OUTPUT_FORMAT(binary)

SECTIONS
{
    .text : {
        *(.entry)
        *(.text*)
        *(.rodata*)
        *(.data*)
    }
}