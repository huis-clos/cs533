/* Thread switching implementation in xassembly to be used
by context_switch.c for project 1 in Jon Walpole's CS533 
Written by Colleen Toth
09 October 2015
*/

.globl thread_switch

thread_switch:
	pushq %rbx
	pushq %rbp
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq %rsp, (%rdi)
	movq (%rsi), %rsp
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbp
	popq %rbx
	ret
