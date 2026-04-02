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
    *(.rodata*)
    . = ALIGN(4);
  } > rom

  __data_lma = LOADADDR(.data);
  .data : AT(ADDR(.text) + SIZEOF(.text))
  {
    __data_start__ = .;
    *(.data*)
    . = ALIGN(4);
    __data_end__ = .;
  } > iwram

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
