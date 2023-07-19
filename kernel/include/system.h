/* vim: tabstop=4 shiftwidth=4 noexpandtab
 */
#ifndef __SYSTEM_H
#define __SYSTEM_H
#define _KERNEL_
#include <types.h>

#define asm __asm__
#define volatile __volatile__

extern unsigned int __irq_sem;

/* Kernel Main */
extern int max(int,int);
extern int min(int,int);
extern int abs(int);
extern void *memmove(void *restrict dest, const void *restrict src, size_t count);
extern void *memset(void *dest, int val, size_t count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
extern uint32_t strlen(const char *str);
extern char * strdup(const char *str);
extern char * strcpy(char * dest, const char * src);
extern int atoi(const char *str);

/* GDT */
extern void gdt_install();
extern void tss_flush();
extern void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access,
			 unsigned char gran);
extern void set_kernel_stack(uintptr_t stack);

typedef struct tss_entry {
	uint32_t	prev_tss;
	uint32_t	esp0;
	uint32_t	ss0;
	uint32_t	esp1;
	uint32_t	ss1;
	uint32_t	esp2;
	uint32_t	ss2;
	uint32_t	cr3;
	uint32_t	eip;
	uint32_t	eflags;
	uint32_t	eax;
	uint32_t	ecx;
	uint32_t	edx;
	uint32_t	ebx;
	uint32_t	esp;
	uint32_t	ebp;
	uint32_t	esi;
	uint32_t	edi;
	uint32_t	es;
	uint32_t	cs;
	uint32_t	ss;
	uint32_t	ds;
	uint32_t	fs;
	uint32_t	gs;
	uint32_t	ldt;
	uint16_t	trap;
	uint16_t	iomap_base;
} __attribute__ ((packed)) tss_entry_t;

#endif
