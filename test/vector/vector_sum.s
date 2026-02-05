	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_v1p0_zicsr2p0_zve32f1p0_zve32x1p0_zve64d1p0_zve64f1p0_zve64x1p0_zvl128b1p0_zvl32b1p0_zvl64b1p0"
	.file	"SysY_Module"
	.globl	main
	.p2align	1
	.type	main,@function
main:
	.cfi_startproc
	addi	sp, sp, -64
	.cfi_def_cfa_offset 64
	vsetivli	zero, 4, e32, m1, ta, ma
	vid.v	v8
	vadd.vi	v9, v8, 1
	addi	a0, sp, 48
	vse32.v	v9, (a0)
	vle32.v	v10, (a0)
	vmv.x.s	a0, v10
	vslidedown.vi	v8, v10, 1
	vmv.x.s	a1, v8
	addw	a0, a0, a1
	vslidedown.vi	v8, v10, 2
	vmv.x.s	a1, v8
	addw	a0, a0, a1
	vslidedown.vi	v8, v10, 3
	vmv.x.s	a1, v8
	addw	a0, a0, a1
	sw	a0, 44(sp)
	vfcvt.f.x.v	v8, v9
	addi	a0, sp, 16
	vse32.v	v8, (a0)
	vle32.v	v9, (a0)
	vfmv.f.s	fa4, v9
	fmv.w.x	fa5, zero
	fadd.s	fa4, fa4, fa5
	vslidedown.vi	v8, v9, 1
	vfmv.f.s	fa3, v8
	fadd.s	fa4, fa4, fa3
	vslidedown.vi	v8, v9, 2
	vfmv.f.s	fa3, v8
	fadd.s	fa4, fa4, fa3
	vslidedown.vi	v8, v9, 3
	vfmv.f.s	fa3, v8
	fadd.s	fa4, fa4, fa3
	fsw	fa4, 12(sp)
	li	a0, 0
	sw	a0, 8(sp)
	flw	fa4, 12(sp)
	flt.s	a0, fa5, fa4
	beqz	a0, .LBB0_2
	j	.LBB0_1
.LBB0_1:
	li	a0, 1
	sw	a0, 8(sp)
	j	.LBB0_2
.LBB0_2:
	lw	a0, 44(sp)
	lw	a1, 8(sp)
	addw	a0, a0, a1
	addi	sp, sp, 64
	ret
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.section	".note.GNU-stack","",@progbits
