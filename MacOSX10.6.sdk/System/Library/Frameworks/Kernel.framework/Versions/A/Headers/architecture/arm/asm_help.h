/*
 * Copyright (c) 2000-2007 Apple Inc. All rights reserved.
 */

#ifndef	_ARCH_ARM_ASM_HELP_H_
#define	_ARCH_ARM_ASM_HELP_H_

#include	<architecture/arm/reg_help.h>


#ifdef	__ASSEMBLER__

#define ALIGN						\
	.align	2

#define	ROUND_TO_STACK(len)				\
	(((len) + STACK_INCR - 1) / STACK_INCR * STACK_INCR)

#ifdef notdef
#define CALL_MCOUNT						\
	pushl	%ebp						;\
	movl	%esp, %ebp					;\
	.data							;\
	1: .long 0						;\
	.text							;\
	lea 9b,%edx						;\
	call mcount						;\
	popl	%ebp						;
#else
#define CALL_MCOUNT
#endif

/*
 * Prologue for functions that may call other functions.  Saves
 * registers and sets up a C frame.
 */
#define NESTED_FUNCTION_PROLOGUE(localvarsize)			\
	.set	__framesize,ROUND_TO_STACK(localvarsize)	;\
	.set	__nested_function, 1				;\
	CALL_MCOUNT						\
	.if __framesize						;\
	  pushl	%ebp						;\
	  movl	%esp, %ebp					;\
	  subl	$__framesize, %esp				;\
	.endif							;\
	pushl	%edi						;\
	pushl	%esi						;\
	pushl	%ebx

/*
 * Prologue for functions that do not call other functions.  Does not
 * save registers (this is the functions responsibility).  Does set
 * up a C frame.
 */
#define LEAF_FUNCTION_PROLOGUE(localvarsize)			\
	.set	__framesize,ROUND_TO_STACK(localvarsize)	;\
	.set	__nested_function, 0				;\
	CALL_MCOUNT						\
	.if __framesize						;\
	  pushl	%ebp						;\
	  movl	%esp, %ebp					;\
	  subl	$__framesize, %esp				;\
	.endif

/*
 * Prologue for any function.
 *
 * We assume that all Leaf functions will be responsible for saving any
 * local registers they clobber.
 */
#define FUNCTION_EPILOGUE					\
	.if __nested_function					;\
	  popl	%ebx						;\
	  popl	%esi						;\
	  popl	%edi						;\
	.endif							;\
	.if __framesize						;\
	  movl	%ebp, %esp					;\
	  popl	%ebp						;\
	.endif							;\
	mov	pc, lr


/*
 * Macros for declaring procedures
 *
 * Use of these macros allows ctags to have a predictable way
 * to find various types of declarations.  They also simplify
 * inserting appropriate symbol table information.
 *
 * NOTE: these simple stubs will be replaced with more
 * complicated versions once we know what the linker and gdb
 * will require as far as register use masks and frame declarations.
 * These macros may also be ifdef'ed in the future to contain profiling
 * code.
 *
 */

/*
 * TEXT -- declare start of text segment
 */
#define	TEXT						\
	.text

/*
 * DATA -- declare start of data segment
 */
#define DATA						\
	.data

/*
 * LEAF -- declare global leaf procedure
 * NOTE: Control SHOULD NOT FLOW into a LEAF!  A LEAF should only
 * be jumped to.  (A leaf may do an align.)  Use a LABEL() if you
 * need control to flow into the label.
 */
#define	LEAF(name, localvarsize)			\
	.globl	name					;\
	ALIGN						;\
name:							;\
	LEAF_FUNCTION_PROLOGUE(localvarsize)

/*
 * X_LEAF -- declare alternate global label for leaf
 */
#define	X_LEAF(name, value)				\
	.globl	name					;\
	.set	name,value

/*
 * P_LEAF -- declare private leaf procedure
 */
#define	P_LEAF(name, localvarsize)			\
	ALIGN						;\
name:							;\
	LEAF_FUNCTION_PROLOGUE(localvarsize)

/*
 * LABEL -- declare a global code label
 * MUST be used (rather than LEAF, NESTED, etc) if control
 * "flows into" the label.
 */
#define	LABEL(name)					\
	.globl	name					;\
name:

/*
 * NESTED -- declare procedure that invokes other procedures
 */
#define	NESTED(name, localvarsize)			\
	.globl	name					;\
	ALIGN						;\
name:							;\
	NESTED_FUNCTION_PROLOGUE(localvarsize)

/*
 * X_NESTED -- declare alternate global label for nested proc
 */
#define	X_NESTED(name, value)				\
	.globl	name					;\
	.set	name,value

/*
 * P_NESTED -- declare private nested procedure
 */
#define	P_NESTED(name, localvarsize)			\
	ALIGN						;\
name:							;\
	NESTED_FUNCTION_PROLOGUE(localvarsize)

/*
 * END -- mark end of procedure
 */
#define	END(name)					\
	FUNCTION_EPILOGUE


/*
 * Storage definition macros
 * The main purpose of these is to allow an easy handle for ctags
 */

/*
 * IMPORT -- import symbol
 */
#define	IMPORT(name)					\
	.reference	name

/*
 * ABS -- declare global absolute symbol
 */
#define	ABS(name, value)				\
	.globl	name					;\
	.set	name,value

/*
 * P_ABS -- declare private absolute symbol
 */
#define	P_ABS(name, value)				\
	.set	name,value

/*
 * EXPORT -- declare global label for data
 */
#define	EXPORT(name)					\
	.globl	name					;\
name:

/*
 * BSS -- declare global zero'ed storage
 */
#define	BSS(name,size)					\
	.comm	name,size


/*
 * P_BSS -- declare private zero'ed storage
 */
#define	P_BSS(name,size)				\
	.lcomm	name,size

/*
 * dynamic/PIC macros for routines which reference external symbols
 */

#if defined(__DYNAMIC__)
#define PICIFY(var)					\
	call	1f					; \
1:							; \
	popl	%edx					; \
	movl	L ## var ## $non_lazy_ptr-1b(%edx),%edx

#define CALL_EXTERN_AGAIN(func)	\
	PICIFY(func)		; \
	call	%edx

#define NON_LAZY_STUB(var)	\
.non_lazy_symbol_pointer	; \
L ## var ## $non_lazy_ptr:	; \
.indirect_symbol var		; \
.long 0				; \
.text

#define CALL_EXTERN(func)	\
	CALL_EXTERN_AGAIN(func)	; \
	NON_LAZY_STUB(func)

#define BRANCH_EXTERN(func)	\
	PICIFY(func)		; \
	jmp	%edx		; \
	NON_LAZY_STUB(func)

#define PUSH_EXTERN(var)	\
	PICIFY(var)		; \
	movl	(%edx),%edx	; \
	pushl	%edx		; \
	NON_LAZY_STUB(var)

#define REG_TO_EXTERN(reg, var)	\
	PICIFY(var)		; \
	movl	reg, (%edx)	; \
	NON_LAZY_STUB(var)

#define EXTERN_TO_REG(var, reg)				\
	call	1f					; \
1:							; \
	popl	%edx					; \
	movl	L ## var ##$non_lazy_ptr-1b(%edx),reg	; \
	NON_LAZY_STUB(var)


#else
#define BRANCH_EXTERN(func)	jmp	func
#define PUSH_EXTERN(var)	pushl	var
#define CALL_EXTERN(func)	call	func
#define CALL_EXTERN_AGAIN(func)	call	func
#define REG_TO_EXTERN(reg, var)	movl	reg, var
#define EXTERN_TO_REG(var, reg)	movl	$ ## var, reg
#endif

#endif	/* __ASSEMBLER__ */

#endif	/* _ARCH_ARM_ASM_HELP_H_ */
