#include <asm.S>
#include <arm.h>
#include <arm32_macros.S>


/* last 5 bit of Program Status Register (PSR) */
	.equ Mode_USR, 0x10 @ User Mode
	.equ Mode_FIQ, 0x11 @ Fast Interrupt Mode
	.equ Mode_IRQ, 0x12 @ Interrupt Mode
	.equ Mode_SVC, 0x13 @ Supervisor Mode
	.equ Mode_ABT, 0x17 @ Abort Mode
	.equ Mode_UND, 0x1B @ Undefined Mode
	.equ Mode_SYS, 0x1F @ System Mode
	.equ Mode_MON, 0x16 @ Monitor Mode

/* 6th and 7th bit of Program Status Register (PSR) */
	.equ I_Bit,    0x80 @ IRQ interrupts disabled
	.equ F_Bit,    0x40 @ FIQ interrupts disabled

	.equ NS_BIT,   0x1

/* memory reserved (in bytes) for stacks of different mode */
	.equ Len_FIQ_Stack,  64
	.equ Len_IRQ_Stack,  64
	.equ Len_ABT_Stack,  64
	.equ Len_UND_Stack,  64
	.equ Len_SVC_Stack,  512
	.equ Len_USR_Stack,  512

    .equ STACK_ADDR, stack_tmp_top

	.equ Offset_FIQ_Stack, 0
	.equ Offset_IRQ_Stack, Offset_FIQ_Stack + Len_FIQ_Stack
	.equ Offset_ABT_Stack, Offset_IRQ_Stack + Len_IRQ_Stack
	.equ Offset_UND_Stack, Offset_ABT_Stack + Len_ABT_Stack
	.equ Offset_SVC_Stack, Offset_UND_Stack + Len_UND_Stack
	.equ Offset_USR_Stack, Offset_SVC_Stack + Len_SVC_Stack

FUNC get_core_pos, :
    mov r0, #0
    bx lr
END_FUNC get_core_pos

FUNC get_mode, :
    mrc p15, 0, r0, c1, c1, 0
    bx lr
END_FUNC get_mode

FUNC plat_cpu_reset_early , :
@	bl CPU_SVC32_MODE
@	bl Disable_MMU_L1cache
@	bl Init_aips
@	bl Init_clock
@	bl setup_stack
@	bl Enable_VFP
	bx	lr
END_FUNC plat_cpu_reset_early

FUNC setup_stack , :
	ldr r0, =STACK_ADDR

	cps #Mode_MON
	ldr sp, =STACK_ADDR

	msr cpsr_c, #Mode_FIQ | I_Bit | F_Bit /* Disable interrupts*/
	sub sp, r0, #Offset_FIQ_Stack

	msr cpsr_c, #Mode_IRQ | I_Bit | F_Bit /* Disable interrupts */
	sub     sp, r0, #Offset_IRQ_Stack

	msr cpsr_c, #Mode_ABT | I_Bit | F_Bit /* Disable interrupts */
	sub sp, r0, #Offset_ABT_Stack

	msr cpsr_c, #Mode_UND | I_Bit | F_Bit
	sub sp, r0, #Offset_UND_Stack

	msr cpsr_c, #Mode_SVC | I_Bit | F_Bit /* Disable interrupts */
	sub sp, r0, #Offset_SVC_Stack

    bx lr
END_FUNC setup_stack

FUNC CPU_SVC32_MODE , :
	mrs r0, cpsr        /* read cpsr register */
	bic r0, r0, #0x1f   /* clear last 5 bits mode */
	orr r0, r0, #0xd3   /* disable FIQ&IRQ and set cpu svc mode*/
	msr cpsr, r0
	bx lr
END_FUNC CPU_SVC32_MODE

FUNC Disable_MMU_L1cache , :
	mrc p15, 0, r0, c1, c0, 0    /* read CP15 register 1 into r0 */
	bic r0, r0, #(0x1<<13)		 /* clear bit 13 */
	bic r0, r0, #(0x1<<12)		 /* disable I Cache */
	bic r0, r0, #(0x1<<2)		 /* disable D Cache */
	bic r0, r0, #(0x1<<0)		 /* disable MMU */
	mcr p15, 0, r0, c1, c0, 0    /* write CP15 register 1 */
	bx lr
END_FUNC Disable_MMU_L1cache

FUNC Init_aips , :
	ldr r0, =0x0207C000
	ldr r1, =0x77777777
	str r1, [r0, #0x0]
	str r1, [r0, #0x4]
	ldr r1, =0x0
	str r1, [r0, #0x40]
	str r1, [r0, #0x44]
	str r1, [r0, #0x48]
	str r1, [r0, #0x4C]
	str r1, [r0, #0x50]

	ldr r0, =0x0217C000
	ldr r1, =0x77777777
	str r1, [r0, #0x0]
	str r1, [r0, #0x4]
	ldr r1, =0x0
	str r1, [r0, #0x40]
	str r1, [r0, #0x44]
	str r1, [r0, #0x48]
	str r1, [r0, #0x4C]
	str r1, [r0, #0x50]
	bx lr
END_FUNC Init_aips

FUNC Init_clock , :
	/* Restore the default values in the Gate registers */
	ldr r0, =0x020c4000
	ldr r1, =0xC0003F
	str r1, [r0, #0x68]
	ldr r1, =0x30FC00
	str r1, [r0, #0x6c]
	ldr r1, =0xFFFC000
	str r1, [r0, #0x70]
	ldr r1, =0x3FF00000
	str r1, [r0, #0x74]
	ldr r1, =0xFFF300
	str r1, [r0, #0x78]
	ldr r1, =0xF0000C3
	str r1, [r0, #0x7c]
	ldr r1, =0x3FC
	str r1, [r0, #0x80]
	bx lr
END_FUNC Init_clock

#ifdef CFG_WITH_VFP

FUNC Enable_VFP , :
    @ r1 = Access Control Register
    MRC p15, #0, r1, c1, c0, #2
    @ enable full access for p10,11
    ORR r1, r1, #(0xf << 20)
    @ ccess Control Register = r1
    MCR p15, #0, r1, c1, c0, #2
    MOV r1, #0
    @ flush prefetch buffer because of FMXR below
    MCR p15, #0, r1, c7, c5, #4
    @ and CP 10 & 11 were only just enabled
    @ Enable VFP itself
    MOV r0,#0x40000000
    @ FPEXC = r0
    FMXR FPEXC, r0
	BX lr
END_FUNC Enable_VFP

#endif
