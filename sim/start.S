.section .text.start
.globl _start
.type _start, @function
_start:
  la sp, __stack_top
  call main

  mv t0, a0
  la a1, __semihost_exit_block
  li t1, 0x20026
  sw t1, 0(a1)
  sw t0, 4(a1)
  li a0, 0x18
  ebreak
1:
  j 1b

.size _start, . - _start

.section .data
.p2align 2
__semihost_exit_block:
  .word 0
  .word 0
