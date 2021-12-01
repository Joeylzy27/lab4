.align 3
.section .text.entry
.equ reg_size, 0x8
.globl trap_s
.globl __switch_to
.globl init_epc
.extern handler_s

trap_s:
	# save sepc and caller-saved registers
	# call handler(scause)
	# load sepc and caller-saved registers
	/*your code*/
	#save callee-saved registers and spec
	addi sp,sp,-248
	sd x1,240(sp)
	sd x3,232(sp)
	sd x4,224(sp)
	sd x5,216(sp)
	sd x6,208(sp)
	sd x7,200(sp)
	sd x8,192(sp)
	sd x9,184(sp)
	sd x10,176(sp)
	sd x11,168(sp)
	sd x12,160(sp)
	sd x13,152(sp)
	sd x14,144(sp)
	sd x15,136(sp)
	sd x16,128(sp)
	sd x17,120(sp)
	sd x18,112(sp)
	sd x19,104(sp)
	sd x20,96(sp)
	sd x21,88(sp)
	sd x22,80(sp)
	sd x23,72(sp)
	sd x24,64(sp)
	sd x25,56(sp)
	sd x26,48(sp)
	sd x27,40(sp)
	sd x28,32(sp)
	sd x29,24(sp)
	sd x30,16(sp)
	sd x31,8(sp)

	csrr a1,sepc
	sd a1,0(sp)
	# call handler_s(scause)
	csrr a0,scause
	call handler_s
	# load sepc and callee-saved registers
	ld a1,0(sp)
	csrw sepc,a1

	ld x1,240(sp)
	ld x3,232(sp)
	ld x4,224(sp)
	ld x5,216(sp)
	ld x6,208(sp)
	ld x7,200(sp)
	ld x8,192(sp)
	ld x9,184(sp)
	ld x10,176(sp)
	ld x11,168(sp)
	ld x12,160(sp)
	ld x13,152(sp)
	ld x14,144(sp)
	ld x15,136(sp)
	ld x16,128(sp)
	ld x17,120(sp)
	ld x18,112(sp)
	ld x19,104(sp)
	ld x20,96(sp)
	ld x21,88(sp)
	ld x22,80(sp)
	ld x23,72(sp)
	ld x24,64(sp)
	ld x25,56(sp)
	ld x26,48(sp)
	ld x27,40(sp)
	ld x28,32(sp)
	ld x29,24(sp)
	ld x30,16(sp)
	ld x31,8(sp)
	
	addi sp,sp,248
	sret

init_epc:
	la t0, dead_loop
	csrw sepc, t0
	ecall
	sret

__switch_to:
	li    a4,  40 
	add   a3, a0, a4
	add   a4, a1, a4
	#Save context into prev->thread
	/*your code*/
	sd ra,0(a3)
	sd sp,8(a3)
	sd s0,16(a3)
	sd s1,24(a3)
	sd s2,32(a3)
	sd s3,40(a3)
	sd s4,48(a3)
	sd s5,56(a3)
	sd s6,64(a3)
	sd s7,72(a3)
	sd s8,80(a3)
	sd s9,88(a3)
	sd s10,96(a3)
	sd s11,104(a3)
	#Restore context from next->thread
	/*your code*/
	ld ra,0(a4)
	ld sp,8(a4)
	ld s0,16(a4)
	ld s1,24(a4)
	ld s2,32(a4)
	ld s3,40(a4)
	ld s4,48(a4)
	ld s5,56(a4)
	ld s6,64(a4)
	ld s7,72(a4)
	ld s8,80(a4)
	ld s9,88(a4)
	ld s10,96(a4)
	ld s11,104(a4)
	#return to ra
	ret
