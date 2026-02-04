	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_v1p0_zicsr2p0_zve32f1p0_zve32x1p0_zve64d1p0_zve64f1p0_zve64x1p0_zvl128b1p0_zvl32b1p0_zvl64b1p0"
	.file	"SysY_Module"
	.globl	main
	.p2align	1
	.type	main,@function
main:
	.cfi_startproc
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
.Lpcrel_hi0:
	auipc	a0, %got_pcrel_hi(a)
	ld	a0, %pcrel_lo(.Lpcrel_hi0)(a0)
	vsetivli	zero, 4, e32, m1, ta, ma
	vle32.v	v9, (a0)
.Lpcrel_hi1:
	auipc	a0, %got_pcrel_hi(b)
	ld	a0, %pcrel_lo(.Lpcrel_hi1)(a0)
	vle32.v	v10, (a0)
	vadd.vv	v8, v9, v10
.Lpcrel_hi2:
	auipc	a1, %got_pcrel_hi(c)
	ld	a1, %pcrel_lo(.Lpcrel_hi2)(a1)
	vse32.v	v8, (a1)
	vle32.v	v9, (a1)
	vle32.v	v10, (a0)
	vmul.vv	v8, v9, v10
	mv	a0, sp
	vse32.v	v8, (a0)
	li	a0, 0
	addi	sp, sp, 16
	ret
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.type	N,@object
	.section	.rodata,"a",@progbits
	.globl	N
	.p2align	2, 0x0
N:
	.word	4
	.size	N, 4

	.type	a,@object
	.data
	.globl	a
	.p2align	4, 0x0
a:
	.word	1
	.word	2
	.word	3
	.word	4
	.size	a, 16

	.type	b,@object
	.globl	b
	.p2align	4, 0x0
b:
	.word	4
	.word	3
	.word	2
	.word	1
	.size	b, 16

	.type	c,@object
	.bss
	.globl	c
	.p2align	4, 0x0
c:
	.zero	16
	.size	c, 16

	.section	".note.GNU-stack","",@progbits
