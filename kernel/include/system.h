/* vim: tabstop=4 shiftwidth=4 noexpandtab
 */
#ifndef __SYSTEM_H
#define __SYSTEM_H
#define _KERNEL_
#include <types.h>
// #include <process.h>

#define STR(x) #x
#define STRSTR(x) STR(x)

/* Binary Literals */
#define b(x) ((uint8_t)b_(0 ## x ## uL))
#define b_(x) ((x & 1) | (x >> 2 & 2) | (x >> 4 & 4) | (x >> 6 & 8) | (x >> 8 & 16) | (x >> 10 & 32) | (x >> 12 & 64) | (x >> 14 & 128))

/* Unimportant Kernel Strings */
#define KERNEL_UNAME "ToAruOS"
#define KERNEL_VERSION_STRING "0.0.1"

#define asm __asm__
#define volatile __volatile__

extern unsigned int __irq_sem;

#define IRQ_OFF { asm volatile ("cli"); }
#define IRQ_RES { asm volatile ("sti"); }
#define PAUSE   { asm volatile ("hlt"); }
#define IRQS_ON_AND_PAUSE { asm volatile ("sti\nhlt\ncli"); }

#define STOP while (1) { PAUSE; }

#define SYSCALL_VECTOR 0x7F
#define SIGNAL_RETURN 0xFFFFDEAF
#define THREAD_RETURN 0xFFFFB00F

extern void * code;
extern void * end;

extern char * boot_arg; /* Argument to pass to init */
extern char * boot_arg_extra; /* Extra data to pass to init */

extern void *sbrk(uintptr_t increment);

extern void tss_flush();

extern void spin_lock(uint8_t volatile * lock);
extern void spin_unlock(uint8_t volatile * lock);

/* Kernel Main */
// extern int max(int,int);
// extern int min(int,int);
// extern int abs(int);
// extern void swap(int *, int *);
// extern void *memcpy(void *restrict dest, const void *restrict src, size_t count);
extern void *memmove(void *restrict dest, const void *restrict src, size_t count);
extern void *memset(void *dest, int val, size_t count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
extern uint32_t strlen(const char *str);
extern char * strdup(const char *str);
extern char * strcpy(char * dest, const char * src);
extern int atoi(const char *str);
// extern unsigned char inportb(unsigned short _port);
// extern void outportb(unsigned short _port, unsigned char _data);
// extern unsigned short inports(unsigned short _port);
// extern void outports(unsigned short _port, unsigned short _data);
// extern unsigned int inportl(unsigned short _port);
// extern void outportl(unsigned short _port, unsigned int _data);
// extern void outportsm(unsigned short port, unsigned char * data, unsigned long size);
// extern void inportsm(unsigned short port, unsigned char * data, unsigned long size);
// extern int strcmp(const char *a, const char *b);
// extern char * strtok_r(char * str, const char * delim, char ** saveptr);
// extern size_t lfind(const char * str, const char accept);
// extern size_t rfind(const char * str, const char accept);
// extern size_t strspn(const char * str, const char * accept);
// extern char * strpbrk(const char * str, const char * accept);
// extern uint32_t krand();
// extern char * strstr(const char * haystack, const char * needle);
// extern uint8_t startswith(const char * str, const char * accept);

// /* VGA driver */
// extern void cls();
// extern void puts(char *str);
// extern void settextcolor(unsigned char forecolor, unsigned char backcolor);
// extern void resettextcolor();
// extern void brighttextcolor();
// extern void init_video();
// extern void placech(unsigned char c, int x, int y, int attr);
// extern void writechf(unsigned char c);
// extern void writech(char c);
// extern void place_csr(uint32_t x, uint32_t y);
// extern void store_csr();
// extern void restore_csr();
// extern void set_serial(int);
// extern void set_csr(int);

/* GDT */
extern void gdt_install();
extern void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access,
			 unsigned char gran);
extern void set_kernel_stack(uintptr_t stack);

/* IDT */
extern void idt_install();
extern void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel,
			 unsigned char flags);

/* Registers */
struct regs {
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int int_no, err_code;
	unsigned int eip, cs, eflags, useresp, ss;
};

typedef struct regs regs_t;

typedef void (*irq_handler_t) (struct regs *);

/* Panic */
#define HALT_AND_CATCH_FIRE(mesg, regs) halt_and_catch_fire(mesg, __FILE__, __LINE__, regs)
#define ASSERT(statement) ((statement) ? (void)0 : assert_failed(__FILE__, __LINE__, #statement))
#define assert(statement) ((statement) ? (void)0 : assert_failed(__FILE__, __LINE__, #statement))
void halt_and_catch_fire(char *error_message, const char *file, int line, struct regs * regs);
void assert_failed(const char *file, uint32_t line, const char *desc);

/* ISRS */
extern void isrs_install();
extern void isrs_install_handler(int isrs, irq_handler_t);
extern void isrs_uninstall_handler(int isrs);

/* Interrupt Handlers */
extern void irq_install();
extern void irq_install_handler(int irq, irq_handler_t);
extern void irq_uninstall_handler(int irq);
extern void irq_gates();
extern void irq_ack();

/* Timer */
extern void timer_install();
extern unsigned long timer_ticks;
extern unsigned char timer_subticks;
extern void relative_time(unsigned long seconds, unsigned long subseconds, unsigned long * out_seconds, unsigned long * out_subseconds);

/* Keyboard */
extern void keyboard_install();
extern void keyboard_reset_ps2();
extern void keyboard_wait();
extern void putch(unsigned char c);

/* Mouse */
extern void mouse_install();
/* shell */
extern void start_shell();

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
