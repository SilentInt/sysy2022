	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_v1p0_zicsr2p0_zve32f1p0_zve32x1p0_zve64d1p0_zve64f1p0_zve64x1p0_zvl128b1p0_zvl32b1p0_zvl64b1p0"
	.file	"SysY_Module"
	.p2align	1
	.type	mix,@function
	.variant_cc	mix
mix:
	.cfi_startproc
	addi	sp, sp, -128
	.cfi_def_cfa_offset 128
	sd	ra, 120(sp)
	sd	s0, 112(sp)
	.cfi_offset ra, -8
	.cfi_offset s0, -16
	addi	s0, sp, 128
	.cfi_def_cfa s0, 0
	andi	sp, sp, -32
	vmv2r.v	v14, v10
	vmv2r.v	v12, v8
	addi	a1, sp, 64
	vsetivli	zero, 8, e32, m2, ta, ma
	vse32.v	v8, (a1)
	addi	a0, sp, 32
	vse32.v	v10, (a0)
	vle32.v	v10, (a1)
	vle32.v	v12, (a0)
	vadd.vv	v8, v10, v12
	mv	a0, sp
	vse32.v	v8, (a0)
	vle32.v	v8, (a0)
	addi	sp, s0, -128
	ld	ra, 120(sp)
	ld	s0, 112(sp)
	addi	sp, sp, 128
	ret
.Lfunc_end0:
	.size	mix, .Lfunc_end0-mix
	.cfi_endproc

	.section	.rodata.cst32,"aM",@progbits,32
	.p2align	5, 0x0
.LCPI1_0:
	.word	1
	.word	1
	.word	2
	.word	3
	.word	5
	.word	8
	.word	13
	.word	21
	.text
	.globl	main
	.p2align	1
	.type	main,@function
main:
	.cfi_startproc
	addi	sp, sp, -128
	.cfi_def_cfa_offset 128
	sd	ra, 120(sp)
	sd	s0, 112(sp)
	.cfi_offset ra, -8
	.cfi_offset s0, -16
	addi	s0, sp, 128
	.cfi_def_cfa s0, 0
	andi	sp, sp, -32
.Lpcrel_hi0:
	auipc	a0, %pcrel_hi(.LCPI1_0)
	addi	a0, a0, %pcrel_lo(.Lpcrel_hi0)
	vsetivli	zero, 8, e32, m2, ta, ma
	vle32.v	v8, (a0)
	addi	a1, sp, 64
	vse32.v	v8, (a1)
	vid.v	v10
	vadd.vi	v8, v10, 1
	addi	a0, sp, 32
	vse32.v	v8, (a0)
	vle32.v	v8, (a1)
	vle32.v	v10, (a0)
	call	mix
	mv	a0, sp
	vsetivli	zero, 8, e32, m2, ta, ma
	vse32.v	v8, (a0)
	li	a0, 0
	addi	sp, s0, -128
	ld	ra, 120(sp)
	ld	s0, 112(sp)
	addi	sp, sp, 128
	ret
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc

	.section	".note.GNU-stack","",@progbits
