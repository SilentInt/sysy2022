	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_v1p0_zicsr2p0_zve32f1p0_zve32x1p0_zve64d1p0_zve64f1p0_zve64x1p0_zvl128b1p0_zvl32b1p0_zvl64b1p0"
	.file	"SysY_Module"
	.globl	main
	.p2align	1
	.type	main,@function
main:
	.cfi_startproc
	addi	sp, sp, -176
	.cfi_def_cfa_offset 176
	vsetivli	zero, 4, e32, m1, ta, ma
	vid.v	v8
	vadd.vi	v9, v8, 1
	addi	a0, sp, 160
	vse32.v	v9, (a0)
	vle32.v	v10, (a0)
	vadd.vi	v8, v10, 3
	addi	a1, sp, 144
	vse32.v	v8, (a1)
	vle32.v	v10, (a0)
	vrsub.vi	v8, v10, 10
	addi	a2, sp, 128
	vse32.v	v8, (a2)
	vle32.v	v10, (a0)
	vadd.vv	v8, v10, v10
	addi	a2, sp, 112
	vse32.v	v8, (a2)
	vle32.v	v11, (a0)
	li	a2, 100
	vmv.v.x	v10, a2
	vdiv.vv	v8, v10, v11
	addi	a2, sp, 96
	vse32.v	v8, (a2)
	vle32.v	v10, (a0)
	vsrl.vi	v11, v10, 31
	vadd.vv	v8, v10, v11
	vand.vi	v11, v8, -2
	vsub.vv	v8, v10, v11
	addi	a0, sp, 80
	vse32.v	v8, (a0)
	vfcvt.f.x.v	v8, v9
	addi	a0, sp, 64
	vse32.v	v8, (a0)
	vle32.v	v9, (a0)
	lui	a2, 258048
	fmv.w.x	fa5, a2
	vfadd.vf	v8, v9, fa5
	addi	a2, sp, 48
	vse32.v	v8, (a2)
	vle32.v	v9, (a0)
	lui	a2, 262656
	fmv.w.x	fa5, a2
	vfmul.vf	v8, v9, fa5
	addi	a2, sp, 32
	vse32.v	v8, (a2)
	vle32.v	v9, (a0)
	lui	a0, 266752
	fmv.w.x	fa5, a0
	vfrdiv.vf	v8, v9, fa5
	addi	a0, sp, 16
	vse32.v	v8, (a0)
	li	a0, 0
	sw	a0, 12(sp)
	vle32.v	v9, (a1)
	vmv.x.s	a1, v9
	vslidedown.vi	v8, v9, 1
	vmv.x.s	a2, v8
	addw	a1, a1, a2
	vslidedown.vi	v8, v9, 2
	vmv.x.s	a2, v8
	addw	a1, a1, a2
	vslidedown.vi	v8, v9, 3
	vmv.x.s	a2, v8
	addw	a1, a1, a2
	bge	a0, a1, .LBB0_2
	j	.LBB0_1
.LBB0_1:
	li	a0, 1
	sw	a0, 12(sp)
	j	.LBB0_2
.LBB0_2:
	addi	a0, sp, 48
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
	flt.s	a0, fa5, fa4
	beqz	a0, .LBB0_4
	j	.LBB0_3
.LBB0_3:
	lw	a0, 12(sp)
	addiw	a0, a0, 1
	sw	a0, 12(sp)
	j	.LBB0_4
.LBB0_4:
	lw	a0, 12(sp)
	addi	sp, sp, 176
	ret
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.section	".note.GNU-stack","",@progbits
