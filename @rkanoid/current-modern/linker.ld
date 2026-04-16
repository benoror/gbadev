ENTRY(_start)

MEMORY
{
  rom   (rx) : ORIGIN = 0x08000000, LENGTH = 32M
  iwram (rwx): ORIGIN = 0x03000000, LENGTH = 32K
}

SECTIONS
{
  .text :
  {
    KEEP(*(.text._start))
    *(.text*)
    . = ALIGN(4);
    __text_end = .;
  } > rom

  .rodata :
  {
    *(.rodata*)
    *(.rodata.*)
    . = ALIGN(4);
  } > rom

  .data : AT(LOADADDR(.rodata) + SIZEOF(.rodata))
  {
    __data_start__ = .;
    *(.data*)
    . = ALIGN(4);
    __data_end__ = .;
  } > iwram
  __data_lma = LOADADDR(.data);

  .bss (NOLOAD) :
  {
    __bss_start__ = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
  } > iwram

  end = .;
  _end = .;

  /DISCARD/ :
  {
    *(.comment*)
    *(.note*)
    *(.eh_frame*)
  }
}
