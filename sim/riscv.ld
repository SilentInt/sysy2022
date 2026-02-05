OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY {
  RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 128M
}

PHDRS {
  text PT_LOAD FLAGS(5); /* R_X */
  data PT_LOAD FLAGS(6); /* R_W */
}

SECTIONS {
  . = ORIGIN(RAM);

  .text : {
    *(.text.start)
    *(.text .text.*)
  } > RAM :text

  .rodata : {
    *(.rodata .rodata.*)
  } > RAM :text

  .data : {
    *(.data .data.*)
  } > RAM :data

  .bss : {
    *(.bss .bss.*)
    *(COMMON)
  } > RAM :data

  . = ALIGN(16);
  __stack_top = ORIGIN(RAM) + LENGTH(RAM);
}
