/*
 * CoreFreq
 * Copyright (C) 2015-2018 CYRIL INGENIERIE
 * Licenses: GPL2
 */

#define RDCOUNTER(_val, _cnt)						\
({									\
	unsigned int _lo, _hi;						\
									\
	asm volatile							\
	(								\
		"rdmsr"							\
		: "=a" (_lo),						\
		  "=d" (_hi)						\
		: "c" (_cnt)						\
	);								\
	_val=_lo | ((unsigned long long) _hi << 32);			\
})

#define WRCOUNTER(_val, _cnt)						\
	asm volatile							\
	(								\
		"wrmsr"							\
		:							\
		: "c" (_cnt),						\
		  "a" ((unsigned int) _val & 0xFFFFFFFF),		\
		  "d" ((unsigned int) (_val >> 32))			\
	);

#define RDMSR(_data, _reg)						\
({									\
	unsigned int _lo, _hi;						\
									\
	asm volatile							\
	(								\
		"rdmsr"							\
		: "=a" (_lo),						\
		  "=d" (_hi)						\
		: "c" (_reg)						\
	);								\
	_data.value=_lo | ((unsigned long long) _hi << 32);		\
})

#define WRMSR(_data, _reg)						\
	asm volatile							\
	(								\
		"wrmsr"							\
		:							\
		: "c" (_reg),						\
		  "a" ((unsigned int) _data.value & 0xFFFFFFFF),	\
		  "d" ((unsigned int) (_data.value >> 32))		\
	);

#define RDMSR64(_data, _reg)						\
	asm volatile							\
	(								\
		"xorq	%%rax, %%rax"	"\n\t"				\
		"xorq	%%rdx, %%rdx"	"\n\t"				\
		"movq	%1,%%rcx"	"\n\t"				\
		"rdmsr"			"\n\t"				\
		"shlq	$32, %%rdx"	"\n\t"				\
		"orq	%%rdx, %%rax"	"\n\t"				\
		"movq	%%rax, %0"					\
		: "=m" (_data)						\
		: "i" (_reg)						\
		: "%rax", "%rcx", "%rdx"				\
	)

#define WRMSR64(_data, _reg)						\
	asm volatile							\
	(								\
		"movq	%0, %%rax"		"\n\t"			\
		"movq	%%rax, %%rdx"		"\n\t"			\
		"shrq	$32, %%rdx"		"\n\t"			\
		"movq	%1, %%rcx"		"\n\t"			\
		"wrmsr"							\
		: "=m" (_data)						\
		: "i" (_reg)						\
		: "%rax", "%rcx", "%rdx"				\
	)

#define ASM_CODE_RDMSR(_msr, _reg) \
	"# Read MSR counter."		"\n\t"		\
	"movq	$" #_msr ", %%rcx"	"\n\t"		\
	"rdmsr"				"\n\t"		\
	"shlq	$32, %%rdx"		"\n\t"		\
	"orq	%%rdx, %%rax"		"\n\t"		\
	"# Save counter value."		"\n\t"		\
	"movq	%%rax, %%" #_reg	"\n\t"

#define ASM_RDMSR(_msr, _reg) ASM_CODE_RDMSR(_msr, _reg)


#define ASM_COUNTERx1(	_reg0, _reg1,			\
			_tsc_inst, mem_tsc,		\
			_msr1, _mem1)			\
asm volatile						\
(							\
	_tsc_inst(_reg0)				\
	ASM_RDMSR(_msr1, _reg1)				\
	"# Store values into memory."	"\n\t"		\
	"movq	%%" #_reg0 ", %0"	"\n\t"		\
	"movq	%%" #_reg1 ", %1"			\
	: "=m" (mem_tsc), "=m" (_mem1)			\
	:						\
	: "%rax", "%rcx", "%rdx",			\
	  "%" #_reg0"", "%" #_reg1"",			\
	  "memory"					\
);


#define ASM_COUNTERx2(	_reg0, _reg1, _reg2,		\
			_tsc_inst, mem_tsc,		\
			_msr1, _mem1, _msr2, _mem2)	\
asm volatile						\
(							\
	_tsc_inst(_reg0)				\
	ASM_RDMSR(_msr1, _reg1)				\
	ASM_RDMSR(_msr2, _reg2)				\
	"# Store values into memory."	"\n\t"		\
	"movq	%%" #_reg0 ", %0"	"\n\t"		\
	"movq	%%" #_reg1 ", %1"	"\n\t"		\
	"movq	%%" #_reg2 ", %2"			\
	: "=m" (mem_tsc), "=m" (_mem1), "=m" (_mem2)	\
	:						\
	: "%rax", "%rcx", "%rdx",			\
	  "%" #_reg0"", "%" #_reg1"", "%" #_reg2"",	\
	  "memory"					\
);


#define ASM_COUNTERx3(	_reg0, _reg1, _reg2, _reg3,			\
			_tsc_inst, mem_tsc,				\
			_msr1, _mem1, _msr2, _mem2, _msr3, _mem3)	\
asm volatile								\
(									\
	_tsc_inst(_reg0)						\
	ASM_RDMSR(_msr1, _reg1)						\
	ASM_RDMSR(_msr2, _reg2)						\
	ASM_RDMSR(_msr3, _reg3)						\
	"# Store values into memory."	"\n\t"				\
	"movq	%%" #_reg0 ", %0"	"\n\t"				\
	"movq	%%" #_reg1 ", %1"	"\n\t"				\
	"movq	%%" #_reg2 ", %2"	"\n\t"				\
	"movq	%%" #_reg3 ", %3"					\
	: "=m" (mem_tsc), "=m" (_mem1), "=m" (_mem2), "=m" (_mem3)	\
	:								\
	: "%rax", "%rcx", "%rdx",					\
	  "%" #_reg0"", "%" #_reg1"", "%" #_reg2"", "%" #_reg3"",	\
	  "memory"							\
);


#define ASM_COUNTERx4(	_reg0, _reg1, _reg2, _reg3, _reg4,		\
			_tsc_inst, mem_tsc,				\
			_msr1, _mem1, _msr2, _mem2, _msr3, _mem3,	\
			_msr4, _mem4)					\
asm volatile								\
(									\
	_tsc_inst(_reg0)						\
	ASM_RDMSR(_msr1, _reg1)						\
	ASM_RDMSR(_msr2, _reg2)						\
	ASM_RDMSR(_msr3, _reg3)						\
	ASM_RDMSR(_msr4, _reg4)						\
	"# Store values into memory."	"\n\t"				\
	"movq	%%" #_reg0 ", %0"	"\n\t"				\
	"movq	%%" #_reg1 ", %1"	"\n\t"				\
	"movq	%%" #_reg2 ", %2"	"\n\t"				\
	"movq	%%" #_reg3 ", %3"	"\n\t"				\
	"movq	%%" #_reg4 ", %4"					\
	: "=m" (mem_tsc), "=m" (_mem1), "=m" (_mem2), "=m" (_mem3),	\
	  "=m" (_mem4)							\
	:								\
	: "%rax", "%rcx", "%rdx",					\
	  "%" #_reg0"", "%" #_reg1"", "%" #_reg2"", "%" #_reg3"",	\
	  "%" #_reg4"",							\
	  "memory"							\
);


#define ASM_COUNTERx5(	_reg0, _reg1, _reg2, _reg3, _reg4, _reg5,	\
			_tsc_inst, mem_tsc,				\
			_msr1, _mem1, _msr2, _mem2, _msr3, _mem3,	\
			_msr4, _mem4, _msr5, _mem5)			\
asm volatile								\
(									\
	_tsc_inst(_reg0)						\
	ASM_RDMSR(_msr1, _reg1)						\
	ASM_RDMSR(_msr2, _reg2)						\
	ASM_RDMSR(_msr3, _reg3)						\
	ASM_RDMSR(_msr4, _reg4)						\
	ASM_RDMSR(_msr5, _reg5)						\
	"# Store values into memory."	"\n\t"				\
	"movq	%%" #_reg0 ", %0"	"\n\t"				\
	"movq	%%" #_reg1 ", %1"	"\n\t"				\
	"movq	%%" #_reg2 ", %2"	"\n\t"				\
	"movq	%%" #_reg3 ", %3"	"\n\t"				\
	"movq	%%" #_reg4 ", %4"	"\n\t"				\
	"movq	%%" #_reg5 ", %5"					\
	: "=m" (mem_tsc), "=m" (_mem1), "=m" (_mem2), "=m" (_mem3),	\
	  "=m" (_mem4), "=m" (_mem5)					\
	:								\
	: "%rax", "%rcx", "%rdx",					\
	  "%" #_reg0"", "%" #_reg1"", "%" #_reg2"", "%" #_reg3"",	\
	  "%" #_reg4"", "%" #_reg5"",					\
	  "memory"							\
);

#define ASM_COUNTERx5_STACK(_reg0, _reg1, _reg2, _reg3, 		\
			_tsc_inst, mem_tsc,				\
			_msr1, _mem1, _msr2, _mem2, _msr3, _mem3,	\
			_msr4, _mem4, _msr5, _mem5)			\
asm volatile								\
(									\
	_tsc_inst(_reg0)						\
	ASM_RDMSR(_msr1, _reg1)						\
	ASM_RDMSR(_msr2, _reg2)						\
	ASM_RDMSR(_msr3, _reg3)						\
	"pushq	%%" #_reg3		"\n\t"				\
	ASM_RDMSR(_msr4, _reg3)						\
	"pushq	%%" #_reg3		"\n\t"				\
	ASM_RDMSR(_msr5, _reg3)						\
	"# Store values into memory."	"\n\t"				\
	"movq	%%" #_reg0 ", %0"	"\n\t"				\
	"movq	%%" #_reg1 ", %1"	"\n\t"				\
	"movq	%%" #_reg2 ", %2"	"\n\t"				\
	"movq	%%" #_reg3 ", %5"	"\n\t"				\
	"popq	%%" #_reg3		"\n\t"				\
	"movq	%%" #_reg3 ", %4"	"\n\t"				\
	"popq	%%" #_reg3		"\n\t"				\
	"movq	%%" #_reg3 ", %3"					\
	: "=m" (mem_tsc), "=m" (_mem1), "=m" (_mem2), "=m" (_mem3),	\
	  "=m" (_mem4), "=m" (_mem5)					\
	:								\
	: "%rax", "%rcx", "%rdx",					\
	  "%" #_reg0"", "%" #_reg1"", "%" #_reg2"", "%" #_reg3"",	\
	  "memory"							\
);


#define ASM_COUNTERx6(	_reg0, _reg1, _reg2, _reg3, _reg4, _reg5, _reg6,\
			_tsc_inst, mem_tsc,				\
			_msr1, _mem1, _msr2, _mem2, _msr3, _mem3,	\
			_msr4, _mem4, _msr5, _mem5, _msr6, _mem6)	\
asm volatile								\
(									\
	_tsc_inst(_reg0)						\
	ASM_RDMSR(_msr1, _reg1)						\
	ASM_RDMSR(_msr2, _reg2)						\
	ASM_RDMSR(_msr3, _reg3)						\
	ASM_RDMSR(_msr4, _reg4)						\
	ASM_RDMSR(_msr5, _reg5)						\
	ASM_RDMSR(_msr6, _reg6)						\
	"# Store values into memory."	"\n\t"				\
	"movq	%%" #_reg0 ", %0"	"\n\t"				\
	"movq	%%" #_reg1 ", %1"	"\n\t"				\
	"movq	%%" #_reg2 ", %2"	"\n\t"				\
	"movq	%%" #_reg3 ", %3"	"\n\t"				\
	"movq	%%" #_reg4 ", %4"	"\n\t"				\
	"movq	%%" #_reg5 ", %5"	"\n\t"				\
	"movq	%%" #_reg6 ", %6"					\
	: "=m" (mem_tsc), "=m" (_mem1), "=m" (_mem2), "=m" (_mem3),	\
	  "=m" (_mem4), "=m" (_mem5), "=m" (_mem6)			\
	:								\
	: "%rax", "%rcx", "%rdx",					\
	  "%" #_reg0"", "%" #_reg1"", "%" #_reg2"", "%" #_reg3"",	\
	  "%" #_reg4"", "%" #_reg5"", "%" #_reg6"",			\
	  "memory"							\
);

#define ASM_COUNTERx6_STACK(_reg0, _reg1, _reg2, _reg3, 		\
			_tsc_inst, mem_tsc,				\
			_msr1, _mem1, _msr2, _mem2, _msr3, _mem3,	\
			_msr4, _mem4, _msr5, _mem5, _msr6, _mem6)	\
asm volatile								\
(									\
	_tsc_inst(_reg0)						\
	ASM_RDMSR(_msr1, _reg1)						\
	ASM_RDMSR(_msr2, _reg2)						\
	ASM_RDMSR(_msr3, _reg3)						\
	"pushq	%%" #_reg3		"\n\t"				\
	ASM_RDMSR(_msr4, _reg3)						\
	"pushq	%%" #_reg3		"\n\t"				\
	ASM_RDMSR(_msr5, _reg3)						\
	"pushq	%%" #_reg3		"\n\t"				\
	ASM_RDMSR(_msr6, _reg3)						\
	"# Store values into memory."	"\n\t"				\
	"movq	%%" #_reg0 ", %0"	"\n\t"				\
	"movq	%%" #_reg1 ", %1"	"\n\t"				\
	"movq	%%" #_reg2 ", %2"	"\n\t"				\
	"movq	%%" #_reg3 ", %6"	"\n\t"				\
	"popq	%%" #_reg3		"\n\t"				\
	"movq	%%" #_reg3 ", %5"	"\n\t"				\
	"popq	%%" #_reg3		"\n\t"				\
	"movq	%%" #_reg3 ", %4"	"\n\t"				\
	"popq	%%" #_reg3		"\n\t"				\
	"movq	%%" #_reg3 ", %3"					\
	: "=m" (mem_tsc), "=m" (_mem1), "=m" (_mem2), "=m" (_mem3),	\
	  "=m" (_mem4), "=m" (_mem5), "=m" (_mem6)			\
	:								\
	: "%rax", "%rcx", "%rdx",					\
	  "%" #_reg0"", "%" #_reg1"", "%" #_reg2"", "%" #_reg3"",	\
	  "memory"							\
);

#define ASM_COUNTERx7(	_reg0, _reg1, _reg2, _reg3,			\
			_reg4, _reg5, _reg6, _reg7,			\
			_tsc_inst, mem_tsc,				\
			_msr1, _mem1, _msr2, _mem2, _msr3, _mem3,	\
			_msr4, _mem4, _msr5, _mem5, _msr6, _mem6,	\
			_msr7, _mem7)					\
asm volatile								\
(									\
	_tsc_inst(_reg0)						\
	ASM_RDMSR(_msr1, _reg1)						\
	ASM_RDMSR(_msr2, _reg2)						\
	ASM_RDMSR(_msr3, _reg3)						\
	ASM_RDMSR(_msr4, _reg4)						\
	ASM_RDMSR(_msr5, _reg5)						\
	ASM_RDMSR(_msr6, _reg6)						\
	ASM_RDMSR(_msr7, _reg7)						\
	"# Store values into memory."	"\n\t"				\
	"movq	%%" #_reg0 ", %0"	"\n\t"				\
	"movq	%%" #_reg1 ", %1"	"\n\t"				\
	"movq	%%" #_reg2 ", %2"	"\n\t"				\
	"movq	%%" #_reg3 ", %3"	"\n\t"				\
	"movq	%%" #_reg4 ", %4"	"\n\t"				\
	"movq	%%" #_reg5 ", %5"	"\n\t"				\
	"movq	%%" #_reg6 ", %6"	"\n\t"				\
	"movq	%%" #_reg7 ", %7"					\
	: "=m" (mem_tsc), "=m" (_mem1), "=m" (_mem2), "=m" (_mem3),	\
	  "=m" (_mem4), "=m" (_mem5), "=m" (_mem6), "=m" (_mem7)	\
	:								\
	: "%rax", "%rcx", "%rdx",					\
	  "%" #_reg0"", "%" #_reg1"", "%" #_reg2"", "%" #_reg3"",	\
	  "%" #_reg4"", "%" #_reg5"", "%" #_reg6"", "%" #_reg7"",	\
	  "memory" \
);

#define ASM_COUNTERx7_STACK(_reg0, _reg1, _reg2,			\
			_tsc_inst, mem_tsc,				\
			_msr1, _mem1, _msr2, _mem2, _msr3, _mem3,	\
			_msr4, _mem4, _msr5, _mem5, _msr6, _mem6,	\
			_msr7, _mem7)					\
asm volatile								\
(									\
	_tsc_inst(_reg0)						\
	ASM_RDMSR(_msr1, _reg1)						\
	ASM_RDMSR(_msr2, _reg2)						\
	"pushq	%%" #_reg2		"\n\t"				\
	ASM_RDMSR(_msr3, _reg2)						\
	"pushq	%%" #_reg2		"\n\t"				\
	ASM_RDMSR(_msr4, _reg2)						\
	"pushq	%%" #_reg2		"\n\t"				\
	ASM_RDMSR(_msr5, _reg2)						\
	"pushq	%%" #_reg2		"\n\t"				\
	ASM_RDMSR(_msr6, _reg2)						\
	"pushq	%%" #_reg2		"\n\t"				\
	ASM_RDMSR(_msr7, _reg2)						\
	"# Store values into memory."	"\n\t"				\
	"movq	%%" #_reg0 ", %0"	"\n\t"				\
	"movq	%%" #_reg1 ", %1"	"\n\t"				\
	"movq	%%" #_reg2 ", %7"	"\n\t"				\
	"popq	%%" #_reg2		"\n\t"				\
	"movq	%%" #_reg2 ", %6"	"\n\t"				\
	"popq	%%" #_reg2		"\n\t"				\
	"movq	%%" #_reg2 ", %5"	"\n\t"				\
	"popq	%%" #_reg2		"\n\t"				\
	"movq	%%" #_reg2 ", %4"	"\n\t"				\
	"popq	%%" #_reg2		"\n\t"				\
	"movq	%%" #_reg2 ", %3"	"\n\t"				\
	"popq	%%" #_reg2		"\n\t"				\
	"movq	%%" #_reg2 ", %2"					\
	: "=m" (mem_tsc), "=m" (_mem1), "=m" (_mem2), "=m" (_mem3),	\
	  "=m" (_mem4), "=m" (_mem5), "=m" (_mem6), "=m" (_mem7)	\
	:								\
	: "%rax", "%rcx", "%rdx",					\
	  "%" #_reg0"", "%" #_reg1"", "%" #_reg2"",			\
	  "memory"							\
);


#define RDTSC_COUNTERx1(mem_tsc, ...) \
ASM_COUNTERx1(r10, r11, ASM_RDTSC, mem_tsc, __VA_ARGS__)

#define RDTSCP_COUNTERx1(mem_tsc, ...) \
ASM_COUNTERx1(r10, r11, ASM_RDTSCP, mem_tsc, __VA_ARGS__)

#define RDTSC_COUNTERx2(mem_tsc, ...) \
ASM_COUNTERx2(r10, r11, r12, ASM_RDTSC, mem_tsc, __VA_ARGS__)

#define RDTSCP_COUNTERx2(mem_tsc, ...) \
ASM_COUNTERx2(r10, r11, r12, ASM_RDTSCP, mem_tsc, __VA_ARGS__)

#define RDTSC_COUNTERx3(mem_tsc, ...) \
ASM_COUNTERx3(r10, r11, r12, r13, ASM_RDTSC, mem_tsc, __VA_ARGS__)

#define RDTSCP_COUNTERx3(mem_tsc, ...) \
ASM_COUNTERx3(r10, r11, r12, r13, ASM_RDTSCP, mem_tsc, __VA_ARGS__)

#define RDTSC_COUNTERx4(mem_tsc, ...) \
ASM_COUNTERx4(r10, r11, r12, r13, r14, ASM_RDTSC, mem_tsc, __VA_ARGS__)

#define RDTSCP_COUNTERx4(mem_tsc, ...) \
ASM_COUNTERx4(r10, r11, r12, r13, r14, ASM_RDTSCP, mem_tsc, __VA_ARGS__)


#if defined(OPTIM_LVL) && OPTIM_LVL == 0

#define RDTSC_COUNTERx5(mem_tsc, ...) \
ASM_COUNTERx5_STACK(r12, r13, r14, r15, ASM_RDTSC, mem_tsc, __VA_ARGS__)

#define RDTSCP_COUNTERx5(mem_tsc, ...) \
ASM_COUNTERx5_STACK(r12, r13, r14, r15, ASM_RDTSCP, mem_tsc, __VA_ARGS__)

#define RDTSC_COUNTERx6(mem_tsc, ...) \
ASM_COUNTERx6_STACK(r12, r13, r14, r15, ASM_RDTSC, mem_tsc, __VA_ARGS__)

#define RDTSCP_COUNTERx6(mem_tsc, ...) \
ASM_COUNTERx6_STACK(r12, r13, r14, r15, ASM_RDTSCP, mem_tsc, __VA_ARGS__)

#define RDTSC_COUNTERx7(mem_tsc, ...) \
ASM_COUNTERx7_STACK(r13, r14, r15, ASM_RDTSC, mem_tsc, __VA_ARGS__)

#define RDTSCP_COUNTERx7(mem_tsc, ...) \
ASM_COUNTERx7_STACK(r13, r14, r15, ASM_RDTSCP, mem_tsc, __VA_ARGS__)

#else
/*	#warning "Optimization"						*/

#define RDTSC_COUNTERx5(mem_tsc, ...) \
ASM_COUNTERx5(r10, r11, r12, r13, r14, r15, ASM_RDTSC, mem_tsc, __VA_ARGS__)

#define RDTSCP_COUNTERx5(mem_tsc, ...) \
ASM_COUNTERx5(r10, r11, r12, r13, r14, r15, ASM_RDTSCP, mem_tsc, __VA_ARGS__)

#define RDTSC_COUNTERx6(mem_tsc, ...) \
ASM_COUNTERx6(r10, r11, r12, r13, r14, r15, r9, ASM_RDTSC, mem_tsc, __VA_ARGS__)

#define RDTSCP_COUNTERx6(mem_tsc, ...) \
ASM_COUNTERx6(r10, r11, r12, r13, r14, r15, r9, ASM_RDTSCP, mem_tsc,__VA_ARGS__)

#define RDTSC_COUNTERx7(mem_tsc, ...) \
ASM_COUNTERx7(r10, r11, r12, r13, r14, r15,r9,r8,ASM_RDTSC,mem_tsc,__VA_ARGS__)

#define RDTSCP_COUNTERx7(mem_tsc, ...) \
ASM_COUNTERx7(r10, r11, r12, r13, r14, r15,r9,r8,ASM_RDTSCP,mem_tsc,__VA_ARGS__)

#endif


#define PCI_CONFIG_ADDRESS(bus, dev, fn, reg) \
	(0x80000000 | (bus << 16) | (dev << 11) | (fn << 8) | (reg & ~3))

#define RDPCI(_data, _reg)						\
({									\
	asm volatile							\
	(								\
		"movl	%1,	%%eax"	"\n\t"				\
		"movl	$0xcf8,	%%edx"	"\n\t"				\
		"outl	%%eax,	%%dx"	"\n\t"				\
		"movl	$0xcfc,	%%edx"	"\n\t"				\
		"inl	%%dx,	%%eax"	"\n\t"				\
		"movl	%%eax,	%0"					\
		: "=m"	(_data)						\
		: "ir"	(_reg)						\
		: "%rax", "%rdx", "memory"				\
	);								\
})

#define WRPCI(_data, _reg)						\
({									\
	asm volatile							\
	(								\
		"movl	%1,	%%eax"	"\n\t"				\
		"movl	$0xcf8,	%%edx"	"\n\t"				\
		"outl	%%eax,	%%dx"	"\n\t"				\
		"movl	%0,	%%eax"	"\n\t"				\
		"movl	$0xcfc,	%%edx"	"\n\t"				\
		"outl	%%eax,	%%dx"					\
		:							\
		: "m"	(_data),					\
		  "ir"	(_reg)						\
		: "%rax", "%rdx", "memory"				\
	);								\
})

typedef struct {
	FEATURES	*Features;
	unsigned int	SMT_Count,
			localProcessor;
	signed int	rc;
} INIT_ARG;

typedef struct {			// V[0] stores previous TSC
	unsigned long long V[2];	// V[1] stores current TSC
} TSC_STRUCT;

#define OCCURRENCES 4
// OCCURRENCES x 2 (TSC values) needs a 64-byte cache line size.
#define STRUCT_SIZE (OCCURRENCES * sizeof(TSC_STRUCT))

typedef struct {
	TSC_STRUCT	*TSC[2];
	CLOCK		Clock;
} COMPUTE_ARG;

typedef struct
{
	struct kmem_cache	*Cache;
	CORE			*Core[];
} KPUBLIC;

enum { CREATED, STARTED, MUSTFWD };

typedef struct
{
	struct hrtimer		Timer;
/*
		TSM: Timer State Machine
			CREATED: 1-0
			STARTED: 2-1
			MUSTFWD: 3-2
*/
	Bit64			TSM __attribute__ ((aligned (64)));
} JOIN;

typedef struct
{
	struct kmem_cache	*Cache;
	JOIN			*Join[];
} KPRIVATE;


/* Sources:
 * Intel® 64 and IA-32 Architectures Software Developer’s Manual; Vol. 2A
 * AMD64 Architecture Programmer’s Manual; Vol. 3
*/

static const CPUID_STRUCT CpuIDforVendor[CPUID_MAX_FUNC]={
/* x86 */
	{.func=0x00000001, .sub=0x00000000},	/* Instruction set	*/
/* Intel */
	{.func=0x00000002, .sub=0x00000000},	/* Cache & TLB		*/
	{.func=0x00000003, .sub=0x00000000},	/* Proc. Serial Number	*/
	{.func=0x00000004, .sub=0x00000000},	/* Cache L1I		*/
	{.func=0x00000004, .sub=0x00000001},	/* Cache L1D		*/
	{.func=0x00000004, .sub=0x00000002},	/* Cache L2		*/
	{.func=0x00000004, .sub=0x00000003},	/* Cache L3		*/
/* x86 */
	{.func=0x00000005, .sub=0x00000000},	/* MONITOR/MWAIT	*/
	{.func=0x00000006, .sub=0x00000000},	/* Power & Thermal Mgmt	*/
	{.func=0x00000007, .sub=0x00000000},	/* Extended Features	*/
/* Intel */
	{.func=0x00000009, .sub=0x00000000},	/* Direct Cache Access	*/
	{.func=0x0000000a, .sub=0x00000000},	/* Perf. Monitoring	*/
/* x86 */
	{.func=0x0000000b, .sub=0x00000000},	/* Ext. Topology	*/
	{.func=0x0000000d, .sub=0x00000000},	/* Ext. State Main leaf	*/
	{.func=0x0000000d, .sub=0x00000001},	/* Ext. State Sub-leaf	*/
/* AMD */
	{.func=0x0000000d, .sub=0x00000002},	/* Ext. State Sub-leaf	*/
/* AMD Family 15h */
	{.func=0x0000000d, .sub=0x0000003e},	/* Ext. State Sub-leaf	*/
/* Intel */
	{.func=0x0000000f, .sub=0x00000000},	/* QoS Monitoring cap.	*/
	{.func=0x0000000f, .sub=0x00000001},	/* L3 QoS Monitoring	*/
	{.func=0x00000010, .sub=0x00000000},	/* QoS Enforcement cap.	*/
	{.func=0x00000010, .sub=0x00000001},	/* L3 Alloc Enumeration	*/
	{.func=0x00000010, .sub=0x00000002},	/* L2 Alloc Enumeration	*/
	{.func=0x00000010, .sub=0x00000003},	/* RAM Bandwidth Enum.	*/
	{.func=0x00000012, .sub=0x00000000},	/* SGX Capability	*/
	{.func=0x00000012, .sub=0x00000001},	/* SGX Attributes	*/
	{.func=0x00000012, .sub=0x00000002},	/* SGX EnclavePageCache	*/
	{.func=0x00000014, .sub=0x00000000},	/* Processor Trace	*/
	{.func=0x00000014, .sub=0x00000001},	/* Proc. Trace Sub-leaf	*/
	{.func=0x00000015, .sub=0x00000000},	/* Time Stamp Counter	*/
	{.func=0x00000016, .sub=0x00000000},	/* Processor Frequency	*/
	{.func=0x00000017, .sub=0x00000000},	/* System-On-Chip	*/
	{.func=0x00000017, .sub=0x00000001},	/* SOC Attrib. Sub-leaf	*/
	{.func=0x00000017, .sub=0x00000002},	/* SOC Attrib. Sub-leaf	*/
	{.func=0x00000017, .sub=0x00000003},	/* SOC Attrib. Sub-leaf	*/
/* x86 */
	{.func=0x80000001, .sub=0x00000000},	/* Extended Features	*/
	{.func=0x80000002, .sub=0x00000000},	/* Processor Name Id.	*/
	{.func=0x80000003, .sub=0x00000000},	/* Processor Name Id.	*/
	{.func=0x80000004, .sub=0x00000000},	/* Processor Name Id.	*/
/* AMD */
	{.func=0x80000005, .sub=0x00000000},	/* Caches L1D L1I TLB	*/
/* x86 */
	{.func=0x80000006, .sub=0x00000000},	/* Cache L2 Size & Way	*/
	{.func=0x80000007, .sub=0x00000000},	/* Advanced Power Mgmt	*/
	{.func=0x80000008, .sub=0x00000000},	/* LM Address Size	*/
/* AMD */
	{.func=0x8000000a, .sub=0x00000000},	/* SVM Revision		*/
	{.func=0x80000019, .sub=0x00000000},	/* Caches & TLB 1G	*/
	{.func=0x8000001a, .sub=0x00000000},	/* Perf. Optimization	*/
	{.func=0x8000001b, .sub=0x00000000},	/* Inst. Based Sampling	*/
	{.func=0x8000001c, .sub=0x00000000},	/* Lightweight Profiling*/
	{.func=0x8000001d, .sub=0x00000000},	/* Cache L1D Properties	*/
	{.func=0x8000001d, .sub=0x00000001},	/* Cache L1I Properties	*/
	{.func=0x8000001d, .sub=0x00000002},	/* Cache L2 Properties	*/
	{.func=0x8000001d, .sub=0x00000003},	/* Cache Properties End	*/
	{.func=0x8000001e, .sub=0x00000000},	/* Extended Identifiers	*/
/* x86 */
	{.func=0x40000000, .sub=0x00000000},	/* Hypervisor vendor	*/
	{.func=0x40000001, .sub=0x00000000},	/* Hypervisor interface	*/
	{.func=0x40000002, .sub=0x00000000},	/* Hypervisor version	*/
	{.func=0x40000003, .sub=0x00000000},	/* Hypervisor features	*/
	{.func=0x40000004, .sub=0x00000000},	/* Hyperv. requirements	*/
	{.func=0x40000005, .sub=0x00000000},	/* Hypervisor limits	*/
	{.func=0x40000006, .sub=0x00000000},	/* Hypervisor exploits	*/
	{.func=0x00000000, .sub=0x00000000},
};

typedef struct {
	char			*brandSubStr;
	unsigned long long	CoreUnlocked	:  1-0,
				UncoreUnlocked	:  2-1,
				freeToUse	: 64-2;
} PROCESSOR_SPECIFIC;

typedef struct
{
	struct	SIGNATURE	Signature;
	void			(*Query)(void);
	void			(*Update)(void *arg);	// Must be static
	void			(*Start)(void *arg);	// Must be static
	void			(*Stop)(void *arg);	// Must be static
	void			(*Exit)(void);
	void			(*Timer)(unsigned int cpu);
	CLOCK			(*BaseClock)(unsigned int ratio);
	long			(*TurboClock)(CLOCK_ARG *pClockMod);
	char			*Architecture;
	unsigned long long	thermalFormula,
				voltageFormula,
				powerFormula;
	struct pci_device_id	*PCI_ids;
	struct {
		void		(*Start)(void *arg);	// Must be static
		void		(*Stop)(void *arg);	// Must be static
	} Uncore;
	PROCESSOR_SPECIFIC	*Specific;
} ARCH;

extern CLOCK BaseClock_GenuineIntel(unsigned int ratio) ;
extern CLOCK BaseClock_AuthenticAMD(unsigned int ratio) ;
extern CLOCK BaseClock_Core(unsigned int ratio) ;
extern CLOCK BaseClock_Core2(unsigned int ratio) ;
extern CLOCK BaseClock_Atom(unsigned int ratio) ;
extern CLOCK BaseClock_Airmont(unsigned int ratio) ;
extern CLOCK BaseClock_Silvermont(unsigned int ratio) ;
extern CLOCK BaseClock_Nehalem(unsigned int ratio) ;
extern CLOCK BaseClock_Westmere(unsigned int ratio) ;
extern CLOCK BaseClock_SandyBridge(unsigned int ratio) ;
extern CLOCK BaseClock_IvyBridge(unsigned int ratio) ;
extern CLOCK BaseClock_Haswell(unsigned int ratio) ;
extern CLOCK BaseClock_Skylake(unsigned int ratio) ;
extern CLOCK BaseClock_AMD_Family_17h(unsigned int ratio) ;

extern long Intel_Turbo_Config8C(CLOCK_ARG *pClockMod) ;
extern long TurboClock_IvyBridge_EP(CLOCK_ARG *pClockMod) ;
extern long TurboClock_Haswell_EP(CLOCK_ARG *pClockMod) ;
extern long TurboClock_Skylake_X(CLOCK_ARG *pClockMod) ;

extern void Query_GenuineIntel(void) ;
static void PerCore_Intel_Query(void *arg) ;
static void Start_GenuineIntel(void *arg) ;
static void Stop_GenuineIntel(void *arg) ;
extern void InitTimer_GenuineIntel(unsigned int cpu) ;

extern void Query_AuthenticAMD(void) ;
static void PerCore_AuthenticAMD_Query(void *arg) ;
static void Start_AuthenticAMD(void *arg) ;
static void Stop_AuthenticAMD(void *arg) ;
extern void InitTimer_AuthenticAMD(unsigned int cpu) ;

extern void Query_Core2(void) ;
static void PerCore_Core2_Query(void *arg) ;
static void Start_Core2(void *arg) ;
static void Stop_Core2(void *arg) ;
extern void InitTimer_Core2(unsigned int cpu) ;

extern void Query_Nehalem(void) ;
static void PerCore_Nehalem_Query(void *arg) ;
static void Start_Nehalem(void *arg) ;
static void Stop_Nehalem(void *arg) ;
extern void InitTimer_Nehalem(unsigned int cpu) ;
static void Start_Uncore_Nehalem(void *arg) ;
static void Stop_Uncore_Nehalem(void *arg) ;

extern void Query_SandyBridge(void) ;
static void PerCore_SandyBridge_Query(void *arg) ;
static void Start_SandyBridge(void *arg) ;
static void Stop_SandyBridge(void *arg) ;
extern void InitTimer_SandyBridge(unsigned int cpu) ;
static void Start_Uncore_SandyBridge(void *arg) ;
static void Stop_Uncore_SandyBridge(void *arg) ;

#define     PerCore_SandyBridge_EP_Query PerCore_SandyBridge_Query
static void Start_SandyBridge_EP(void *arg) ;
static void Stop_SandyBridge_EP(void *arg) ;
extern void InitTimer_SandyBridge_EP(unsigned int cpu) ;
static void Start_Uncore_SandyBridge_EP(void *arg) ;
static void Stop_Uncore_SandyBridge_EP(void *arg) ;

extern void Query_IvyBridge(void) ;
#define     PerCore_IvyBridge_Query PerCore_SandyBridge_Query
extern void Query_IvyBridge_EP(void) ;
#define     PerCore_IvyBridge_EP_Query PerCore_SandyBridge_EP_Query

extern void Query_Haswell(void) ;

extern void Query_Haswell_EP(void) ;
static void PerCore_Haswell_EP_Query(void *arg) ;
static void Start_Haswell_EP(void *arg) ;
static void Stop_Haswell_EP(void *arg) ;
extern void InitTimer_Haswell_EP(unsigned int cpu) ;
static void Start_Uncore_Haswell_EP(void *arg) ;
static void Stop_Uncore_Haswell_EP(void *arg) ;

static void PerCore_Haswell_ULT_Query(void *arg) ;
static void Start_Haswell_ULT(void *arg) ;
static void Stop_Haswell_ULT(void *arg) ;
extern void InitTimer_Haswell_ULT(unsigned int cpu) ;
static void Start_Uncore_Haswell_ULT(void *arg) ;
static void Stop_Uncore_Haswell_ULT(void *arg) ;

extern void Query_Broadwell(void) ;
#define     PerCore_Broadwell_Query PerCore_SandyBridge_Query
#define     Start_Broadwell Start_SandyBridge
#define     Stop_Broadwell Stop_SandyBridge
#define     InitTimer_Broadwell InitTimer_SandyBridge
#define     Start_Uncore_Broadwell Start_Uncore_SandyBridge
#define     Stop_Uncore_Broadwell Stop_Uncore_SandyBridge

#define     PerCore_Skylake_Query PerCore_SandyBridge_Query
static void Start_Skylake(void *arg) ;
static void Stop_Skylake(void *arg) ;
extern void InitTimer_Skylake(unsigned int cpu) ;
static void Start_Uncore_Skylake(void *arg) ;
static void Stop_Uncore_Skylake(void *arg) ;

extern void Query_Skylake_X(void) ;
#define     PerCore_Skylake_X_Query PerCore_SandyBridge_Query
static void Start_Skylake_X(void *arg) ;
static void Stop_Skylake_X(void *arg) ;
extern void InitTimer_Skylake_X(unsigned int cpu) ;
static void Start_Uncore_Skylake_X(void *arg) ;
static void Stop_Uncore_Skylake_X(void *arg) ;

extern void Query_AMD_Family_0Fh(void) ;
static void PerCore_AMD_Family_0Fh_Query(void *arg) ;
static void Start_AMD_Family_0Fh(void *arg) ;
static void Stop_AMD_Family_0Fh(void *arg) ;
extern void InitTimer_AMD_Family_0Fh(unsigned int cpu) ;

extern void Query_AMD_Family_10h(void) ;
static void PerCore_AMD_Family_10h_Query(void *arg) ;
static void Start_AMD_Family_10h(void *arg) ;
static void Stop_AMD_Family_10h(void *arg) ;

extern void Query_AMD_Family_11h(void) ;
#define     PerCore_AMD_Family_11h_Query PerCore_AMD_Family_10h_Query
#define     Start_AMD_Family_11h Start_AMD_Family_10h
#define     Stop_AMD_Family_11h Stop_AMD_Family_10h

extern void Query_AMD_Family_12h(void) ;
#define     PerCore_AMD_Family_12h_Query PerCore_AMD_Family_10h_Query

extern void Query_AMD_Family_14h(void) ;
#define     PerCore_AMD_Family_14h_Query PerCore_AMD_Family_10h_Query

extern void Query_AMD_Family_15h(void) ;
#define     PerCore_AMD_Family_15h_Query PerCore_AMD_Family_10h_Query

extern void Query_AMD_Family_17h(void) ;
static void PerCore_AMD_Family_17h_Query(void *arg) ;
static void Start_AMD_Family_17h(void *arg) ;
static void Stop_AMD_Family_17h(void *arg) ;
extern void InitTimer_AMD_Family_17h(unsigned int cpu) ;

//	[Void]
#define _Void_Signature {.ExtFamily=0x0, .Family=0x0, .ExtModel=0x0, .Model=0x0}

//	[Core]		06_0Eh (32 bits)
#define _Core_Yonah	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x0, .Model=0xE}

//	[Core2]		06_0Fh, 06_15h, 06_16h, 06_17h, 06_1Dh
#define _Core_Conroe	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x0, .Model=0xF}
#define _Core_Kentsfield \
			{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x1, .Model=0x5}
#define _Core_Conroe_616 \
			{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x1, .Model=0x6}
#define _Core_Yorkfield {.ExtFamily=0x0, .Family=0x6, .ExtModel=0x1, .Model=0x7}
#define _Core_Dunnington \
			{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x1, .Model=0xD}

//	[Atom]		06_1Ch, 06_26h, 06_27h (32bits), 06_35h (32bits), 06_36h
#define _Atom_Bonnell	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x1, .Model=0xC}
#define _Atom_Silvermont \
			{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x2, .Model=0x6}
#define _Atom_Lincroft	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x2, .Model=0x7}
#define _Atom_Clovertrail \
			{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x3, .Model=0x5}
#define _Atom_Saltwell	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x3, .Model=0x6}

//	[Silvermont]	06_37h
#define _Silvermont_637	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x3, .Model=0x7}

//	[Avoton]	06_4Dh
#define _Atom_Avoton	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x4, .Model=0xD}

//	[Airmont]	06_4Ch
#define _Atom_Airmont	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x4, .Model=0xC}
//	[Goldmont]	06_5Ch
#define _Atom_Goldmont	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x5, .Model=0xC}
//	[SoFIA]		06_5Dh
#define _Atom_Sofia	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x5, .Model=0xD}
//	[Merrifield]	06_4Ah
#define _Atom_Merrifield \
			{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x4, .Model=0xA}
//	[Moorefield]	06_5Ah
#define _Atom_Moorefield \
			{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x5, .Model=0xA}

//	[Nehalem]	06_1Ah, 06_1Eh, 06_1Fh, 06_2Eh
#define _Nehalem_Bloomfield \
			{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x1, .Model=0xA}
#define _Nehalem_Lynnfield \
			{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x1, .Model=0xE}
#define _Nehalem_MB	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x1, .Model=0xF}
#define _Nehalem_EX	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x2, .Model=0xE}

//	[Westmere]	06_25h, 06_2Ch, 06_2Fh
#define _Westmere	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x2, .Model=0x5}
#define _Westmere_EP	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x2, .Model=0xC}
#define _Westmere_EX	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x2, .Model=0xF}

//	[Sandy Bridge]	06_2Ah, 06_2Dh
#define _SandyBridge	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x2, .Model=0xA}
#define _SandyBridge_EP {.ExtFamily=0x0, .Family=0x6, .ExtModel=0x2, .Model=0xD}

//	[Ivy Bridge]	06_3Ah, 06_3Eh
#define _IvyBridge	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x3, .Model=0xA}
#define _IvyBridge_EP	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x3, .Model=0xE}

//	[Haswell]	06_3Ch, 06_3Fh, 06_45h, 06_46h
#define _Haswell_DT	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x3, .Model=0xC}
#define _Haswell_EP	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x3, .Model=0xF}
#define _Haswell_ULT	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x4, .Model=0x5}
#define _Haswell_ULX	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x4, .Model=0x6}

//	[Broadwell]	06_3Dh, 06_56h, 06_47h, 06_4Fh
#define _Broadwell	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x3, .Model=0xD}
#define _Broadwell_D	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x5, .Model=0x6}
#define _Broadwell_H	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x4, .Model=0x7}
#define _Broadwell_EP	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x4, .Model=0xF}

//	[Skylake]	06_4Eh, 06_5Eh, 06_55h
#define _Skylake_UY	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x4, .Model=0xE}
#define _Skylake_S	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x5, .Model=0xE}
#define _Skylake_X	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x5, .Model=0x5}

//	[Xeon Phi]	06_57h, 06_85h
#define _Xeon_Phi	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x5, .Model=0x7}

//	[Kabylake]	06_8Eh, 06_9Eh
#define _Kabylake	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x9, .Model=0xE}
#define _Kabylake_UY	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x8, .Model=0xE}

//	[Cannonlake]	06_66h
#define _Cannonlake	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x6, .Model=0x6}

//	[Geminilake]	06_7Ah
#define _Geminilake	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x7, .Model=0xA}

//	[Icelake]	06_7Eh
#define _Icelake_UY	{.ExtFamily=0x0, .Family=0x6, .ExtModel=0x7, .Model=0xE}

//	[Family 0Fh]	0F_00h
#define _AMD_Family_0Fh {.ExtFamily=0x0, .Family=0xF, .ExtModel=0x0, .Model=0x0}

//	[Family 10h]	1F_00h
#define _AMD_Family_10h {.ExtFamily=0x1, .Family=0xF, .ExtModel=0x0, .Model=0x0}

//	[Family 11h]	2F_00h
#define _AMD_Family_11h {.ExtFamily=0x2, .Family=0xF, .ExtModel=0x0, .Model=0x0}

//	[Family 12h]	3F_00h
#define _AMD_Family_12h {.ExtFamily=0x3, .Family=0xF, .ExtModel=0x0, .Model=0x0}

//	[Family 14h]	5F_00h
#define _AMD_Family_14h {.ExtFamily=0x5, .Family=0xF, .ExtModel=0x0, .Model=0x0}

//	[Family 15h]	6F_00h
#define _AMD_Family_15h {.ExtFamily=0x6, .Family=0xF, .ExtModel=0x0, .Model=0x0}

//	[Family 16h]	7F_00h
#define _AMD_Family_16h {.ExtFamily=0x7, .Family=0xF, .ExtModel=0x0, .Model=0x0}

//	[Family 17h]	8F_00h
#define _AMD_Family_17h {.ExtFamily=0x8, .Family=0xF, .ExtModel=0x0, .Model=0x0}


typedef kernel_ulong_t (*PCI_CALLBACK)(struct pci_dev *);

static PCI_CALLBACK P945(struct pci_dev *dev) ;
static PCI_CALLBACK P955(struct pci_dev *dev) ;
static PCI_CALLBACK P965(struct pci_dev *dev) ;
static PCI_CALLBACK G965(struct pci_dev *dev) ;
static PCI_CALLBACK P35(struct pci_dev *dev) ;
static PCI_CALLBACK Bloomfield_IMC(struct pci_dev *dev) ;
static PCI_CALLBACK Lynnfield_IMC(struct pci_dev *dev) ;
static PCI_CALLBACK Westmere_EP_IMC(struct pci_dev *dev) ;
static PCI_CALLBACK NHM_IMC_TR(struct pci_dev *dev) ;
static PCI_CALLBACK NHM_NON_CORE(struct pci_dev *dev) ;
static PCI_CALLBACK X58_VTD(struct pci_dev *dev) ;
static PCI_CALLBACK X58_QPI(struct pci_dev *dev) ;
static PCI_CALLBACK SNB_IMC(struct pci_dev *dev) ;
static PCI_CALLBACK IVB_IMC(struct pci_dev *dev) ;
static PCI_CALLBACK HSW_IMC(struct pci_dev *dev) ;
static PCI_CALLBACK SKL_IMC(struct pci_dev *dev) ;
static PCI_CALLBACK AMD_0Fh_MCH(struct pci_dev *dev) ;
static PCI_CALLBACK AMD_0Fh_HTT(struct pci_dev *dev) ;
/* ToDo:
static PCI_CALLBACK SKL_SA(struct pci_dev *dev) ;
static PCI_CALLBACK AMD_IOMMU(struct pci_dev *dev) ;
*/
static struct pci_device_id PCI_Void_ids[] = {
	{0, }
};

static struct pci_device_id PCI_Core2_ids[] = {
	{	// 82945G - Lakeport
		PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_82945P_HB),
		.driver_data = (kernel_ulong_t) P945
	},
	{	// 82945GM - Lakeport
		PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_82945GM_HB),
		.driver_data = (kernel_ulong_t) P945
	},
	{	// 82945GME/SE - Calistoga
	      PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_82945GME_HB),
		.driver_data = (kernel_ulong_t) P945
	},
	{	// 82955X - Lakeport
		PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_82955_HB),
		.driver_data = (kernel_ulong_t) P955
	},
	{	// 946PL/946GZ - Lakeport
		PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_82946GZ_HB),
		.driver_data = (kernel_ulong_t) P965
	},
	{	// Q963/Q965 - Broadwater
		PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_82965Q_HB),
		.driver_data = (kernel_ulong_t) P965
	},
	{	// P965/G965 - Broadwater
		PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_82965G_HB),
		.driver_data = (kernel_ulong_t) P965
	},
	{	// GM965 - Crestline
		PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_82965GM_HB),
		.driver_data = (kernel_ulong_t) G965
	},
	{	// GME965 - Crestline
	      PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_82965GME_HB),
		.driver_data = (kernel_ulong_t) G965
	},
	{	// GM45 - Cantiga
		PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_GM45_HB),
		.driver_data = (kernel_ulong_t) G965
	},
	{	// Q35 - Bearlake
		PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_Q35_HB),
		.driver_data = (kernel_ulong_t) P35
	},
	{	// P35/G33 - Bearlake
		PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_G33_HB),
		.driver_data = (kernel_ulong_t) P35
	},
	{	// Q33 - Bearlake
		PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_Q33_HB),
		.driver_data = (kernel_ulong_t) P35
	},
	{	// X38/X48 - Bearlake
		PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_X38_HB),
		.driver_data = (kernel_ulong_t) P35
	},
	{	// 3200/3210 - Unknown
		PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_3200_HB),
		.driver_data = (kernel_ulong_t) P35
	},
	{	// Q45/Q43 - Eaglelake
		PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_Q45_HB),
		.driver_data = (kernel_ulong_t) P35
	},
	{	// P45/G45 - Eaglelake
		PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_G45_HB),
		.driver_data = (kernel_ulong_t) P35
	},
	{	// G41 - Eaglelake
		PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_G41_HB),
		.driver_data = (kernel_ulong_t) P35
	},
	{0, }
};

	// 1st Generation
static struct pci_device_id PCI_Nehalem_QPI_ids[] = {
	{	// Bloomfield IMC
		PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_I7_MCR),
		.driver_data = (kernel_ulong_t) Bloomfield_IMC
	},
	{	// Bloomfield IMC Test Registers
		PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_I7_MC_TEST),
		.driver_data = (kernel_ulong_t) NHM_IMC_TR
	},
	{	// Nehalem Control Status and RAS Registers
	    PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_X58_HUB_CTRL),
		.driver_data = (kernel_ulong_t) X58_QPI
	},
	{
	    PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_X58_HUB_CORE),
		.driver_data = (kernel_ulong_t) X58_VTD
	},
	{	// Nehalem Bloomfield/Xeon C3500: Non-Core Registers
	PCI_DEVICE(PCI_VENDOR_ID_INTEL,PCI_DEVICE_ID_INTEL_BLOOMFIELD_NON_CORE),
		.driver_data = (kernel_ulong_t) NHM_NON_CORE
	},
	{	// Nehalem EP Xeon C5500: Non-Core Registers
	    PCI_DEVICE(PCI_VENDOR_ID_INTEL,PCI_DEVICE_ID_INTEL_C5500_NON_CORE),
		.driver_data = (kernel_ulong_t) NHM_NON_CORE
	},
	{0, }
};

static struct pci_device_id PCI_Nehalem_DMI_ids[] = {
	{	// Lynnfield IMC
	      PCI_DEVICE(PCI_VENDOR_ID_INTEL,PCI_DEVICE_ID_INTEL_LYNNFIELD_MCR),
		.driver_data = (kernel_ulong_t) Lynnfield_IMC
	},
	{	// Lynnfield IMC Test Registers
	  PCI_DEVICE(PCI_VENDOR_ID_INTEL,PCI_DEVICE_ID_INTEL_LYNNFIELD_MC_TEST),
		.driver_data = (kernel_ulong_t) NHM_IMC_TR
	},
	{	// Lynnfield QuickPath Architecture Generic Non-core Registers
	PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_LYNNFIELD_NON_CORE),
		.driver_data = (kernel_ulong_t) NHM_NON_CORE
	},
	{	// Westmere/Clarkdale QuickPath Architecture Non-core Registers
	PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_CLARKDALE_NON_CORE),
		.driver_data = (kernel_ulong_t) NHM_NON_CORE
	},
	{0, }
};

static struct pci_device_id PCI_Westmere_EP_ids[] = {
	{	// Westmere EP IMC
		PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_NHM_EP_MCR),
		.driver_data = (kernel_ulong_t) Westmere_EP_IMC
	},
	{	// Westmere EP IMC Test Registers
	    PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_NHM_EP_MC_TEST),
		.driver_data = (kernel_ulong_t) NHM_IMC_TR
	},
	{	// Nehalem Control Status and RAS Registers
	    PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_X58_HUB_CTRL),
		.driver_data = (kernel_ulong_t) X58_QPI
	},
	{
	    PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_X58_HUB_CORE),
		.driver_data = (kernel_ulong_t) X58_VTD
	},
	{	// Westmere EP: Non-Core Registers
	    PCI_DEVICE(PCI_VENDOR_ID_INTEL,PCI_DEVICE_ID_INTEL_NHM_EP_NON_CORE),
		.driver_data = (kernel_ulong_t) NHM_NON_CORE
	},
	{0, }
};

	// 2nd Generation
	// Sandy Bridge ix-2xxx, Xeon E3-E5: IMC_HA=0x3ca0 / IMC_TA=0x3ca8 /
	// TA0=0x3caa, TA1=0x3cab / TA2=0x3cac / TA3=0x3cad / TA4=0x3cae
static struct pci_device_id PCI_SandyBridge_ids[] = {
	{
	    PCI_DEVICE(PCI_VENDOR_ID_INTEL,PCI_DEVICE_ID_INTEL_SBRIDGE_IMC_HA0),
		.driver_data = (kernel_ulong_t) SNB_IMC
	},
	{	// Desktop: IMC_SystemAgent=0x0100,0x0104
	    PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_SBRIDGE_IMC_SA),
		.driver_data = (kernel_ulong_t) SNB_IMC
	},
	{
	  PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_SBRIDGE_IMC_0104),
		.driver_data = (kernel_ulong_t) SNB_IMC
	},
	{0, }
};

	// 3rd Generation
	// Ivy Bridge ix-3xxx, Xeon E7/E5 v2: IMC_HA=0x0ea0 / IMC_TA=0x0ea8
	// TA0=0x0eaa / TA1=0x0eab / TA2=0x0eac / TA3=0x0ead
static struct pci_device_id PCI_IvyBridge_ids[] = {
	{
	    PCI_DEVICE(PCI_VENDOR_ID_INTEL,PCI_DEVICE_ID_INTEL_IBRIDGE_IMC_HA0),
		.driver_data = (kernel_ulong_t) IVB_IMC
	},
	{	// Desktop: IMC_SystemAgent=0x0150
	    PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_IBRIDGE_IMC_SA),
		.driver_data = (kernel_ulong_t) IVB_IMC
	},
	{	// Mobile i5-3337U: IMC=0x0154
	  PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_IBRIDGE_IMC_0154),
		.driver_data = (kernel_ulong_t) IVB_IMC
	},
	{0, }
};

	// 4th Generation
	// Haswell ix-4xxx, Xeon E7/E5 v3: IMC_HA0=0x2fa0 / IMC_HA0_TA=0x2fa8
	// TAD0=0x2faa / TAD1=0x2fab / TAD2=0x2fac / TAD3=0x2fad
static struct pci_device_id PCI_Haswell_ids[] = {
	{
	    PCI_DEVICE(PCI_VENDOR_ID_INTEL,PCI_DEVICE_ID_INTEL_HASWELL_IMC_HA0),
		.driver_data = (kernel_ulong_t) HSW_IMC
	},
	{	// Desktop: IMC_SystemAgent=0x0c00
	    PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_HASWELL_IMC_SA),
		.driver_data = (kernel_ulong_t) HSW_IMC
	},
	{0, }
};

	// 5th Generation
	// Broadwell ix-5xxx: IMC_HA0=0x1604 / 0x1614
static struct pci_device_id PCI_Broadwell_ids[] = {
	{
	  PCI_DEVICE(PCI_VENDOR_ID_INTEL,PCI_DEVICE_ID_INTEL_BROADWELL_IMC_HA0),
		.driver_data = (kernel_ulong_t) HSW_IMC
	},
	{
	PCI_DEVICE(PCI_VENDOR_ID_INTEL,PCI_DEVICE_ID_INTEL_BROADWELL_H_IMC_HA0),
		.driver_data = (kernel_ulong_t) HSW_IMC
	},
	{	// Desktop: IMC_SystemAgent=0x0c00
	    PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_HASWELL_IMC_SA),
		.driver_data = (kernel_ulong_t) HSW_IMC
	},
	{0, }
};

	// 6th Generation
static struct pci_device_id PCI_Skylake_ids[] = {
	{
	  PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_SKYLAKE_U_IMC_HA),
		.driver_data = (kernel_ulong_t) SKL_IMC
	},
	{
	  PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_SKYLAKE_Y_IMC_HA),
		.driver_data = (kernel_ulong_t) SKL_IMC
	},
	{
	  PCI_DEVICE(PCI_VENDOR_ID_INTEL,PCI_DEVICE_ID_INTEL_SKYLAKE_S_IMC_HAD),
		.driver_data = (kernel_ulong_t) SKL_IMC
	},
	{
	  PCI_DEVICE(PCI_VENDOR_ID_INTEL,PCI_DEVICE_ID_INTEL_SKYLAKE_S_IMC_HAQ),
		.driver_data = (kernel_ulong_t) SKL_IMC
	},
	{
	  PCI_DEVICE(PCI_VENDOR_ID_INTEL,PCI_DEVICE_ID_INTEL_SKYLAKE_H_IMC_HAD),
		.driver_data = (kernel_ulong_t) SKL_IMC
	},
	{
	  PCI_DEVICE(PCI_VENDOR_ID_INTEL,PCI_DEVICE_ID_INTEL_SKYLAKE_H_IMC_HAQ),
		.driver_data = (kernel_ulong_t) SKL_IMC
	},
	{0, }
};

static struct pci_device_id PCI_Skylake_X_ids[] = {
	{0, }
};

	// 7th & 8th Generation
static struct pci_device_id PCI_Kabylake_ids[] = {
	{
	PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_KABYLAKE_H_IMC_HAD),
		.driver_data = (kernel_ulong_t) SKL_IMC
	},
	{
	PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_KABYLAKE_U_IMC_HA),
		.driver_data = (kernel_ulong_t) SKL_IMC
	},
	{
	PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_KABYLAKE_Y_IMC_HA),
		.driver_data = (kernel_ulong_t) SKL_IMC
	},
	{
	PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_KABYLAKE_S_IMC_HAD),
		.driver_data = (kernel_ulong_t) SKL_IMC
	},
	{
	PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_KABYLAKE_H_IMC_HAQ),
		.driver_data = (kernel_ulong_t) SKL_IMC
	},
	{
	PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_KABYLAKE_U_IMC_HAQ),
		.driver_data = (kernel_ulong_t) SKL_IMC
	},
	{
	PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_KABYLAKE_S_IMC_HAQ),
		.driver_data = (kernel_ulong_t) SKL_IMC
	},
	{
	PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_KABYLAKE_X_IMC_HAQ),
		.driver_data = (kernel_ulong_t) SKL_IMC
	},
	{
      PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_COFFEELAKE_S_IMC_HAQ),
		.driver_data = (kernel_ulong_t) SKL_IMC
	},
	{
      PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_COFFEELAKE_S_IMC_HAH),
		.driver_data = (kernel_ulong_t) SKL_IMC
	},
	{0, }
};

	// AMD Family 0Fh
static struct pci_device_id PCI_AMD_0Fh_ids[] = {
	{
		PCI_DEVICE(PCI_VENDOR_ID_AMD, PCI_DEVICE_ID_AMD_K8_NB_MEMCTL),
		.driver_data = (kernel_ulong_t) AMD_0Fh_MCH
	},
	{
		PCI_DEVICE(PCI_VENDOR_ID_AMD, PCI_DEVICE_ID_AMD_K8_NB),
		.driver_data = (kernel_ulong_t) AMD_0Fh_HTT
	},
	{0, }
};

	// AMD Family 17h
static struct pci_device_id PCI_AMD_17h_ids[] = {
	{0, }
};


static PROCESSOR_SPECIFIC Void_Specific[] = {
	{NULL, 0}
};

static PROCESSOR_SPECIFIC Nehalem_Bloomfield_Specific[] = {
	{
	.brandSubStr = "Intel(R) Core(TM) i7 CPU 920",
	.CoreUnlocked = 0,
	.UncoreUnlocked = 0
	},
	{NULL, 0}
};

static PROCESSOR_SPECIFIC Kabylake_Specific[] = {
	{
	.brandSubStr = "Intel(R) Core(TM) i5-7500 CPU",
	.CoreUnlocked = 0,
	.UncoreUnlocked = 1
	},
	{NULL, 0}
};


static ARCH Arch[ARCHITECTURES] = {
/*  0*/	{
	.Signature = _Void_Signature,
	.Query = NULL,
	.Update = NULL,
	.Start = NULL,
	.Stop = NULL,
	.Exit = NULL,
	.Timer = NULL,
	.BaseClock = NULL,
	.TurboClock = NULL,
	.Architecture = NULL,
	.thermalFormula = THERMAL_FORMULA_NONE,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_NONE,
	.PCI_ids = PCI_Void_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},
/*  1*/	{
	.Signature = _Core_Yonah,
	.Query = Query_GenuineIntel,
	.Update = PerCore_Intel_Query,
	.Start = Start_GenuineIntel,
	.Stop = Stop_GenuineIntel,
	.Exit = NULL,
	.Timer = InitTimer_GenuineIntel,
	.BaseClock = BaseClock_Core,
	.TurboClock = NULL,
	.Architecture = "Core/Yonah",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_NONE,
	.PCI_ids = PCI_Core2_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},
/*  2*/	{
	.Signature = _Core_Conroe,
	.Query = Query_Core2,
	.Update = PerCore_Core2_Query,
	.Start = Start_Core2,
	.Stop = Stop_Core2,
	.Exit = NULL,
	.Timer = InitTimer_Core2,
	.BaseClock = BaseClock_Core2,
	.TurboClock = NULL,
	.Architecture = "Core2/Conroe/Merom",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_INTEL_MEROM,
	.powerFormula   = POWER_FORMULA_NONE,
	.PCI_ids = PCI_Core2_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},
/*  3*/	{
	.Signature = _Core_Kentsfield,
	.Query = Query_Core2,
	.Update = PerCore_Core2_Query,
	.Start = Start_Core2,
	.Stop = Stop_Core2,
	.Exit = NULL,
	.Timer = InitTimer_Core2,
	.BaseClock = BaseClock_Core2,
	.TurboClock = NULL,
	.Architecture = "Core2/Kentsfield",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_NONE,
	.PCI_ids = PCI_Core2_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},
/*  4*/	{
	.Signature = _Core_Conroe_616,
	.Query = Query_Core2,
	.Update = PerCore_Core2_Query,
	.Start = Start_Core2,
	.Stop = Stop_Core2,
	.Exit = NULL,
	.Timer = InitTimer_Core2,
	.BaseClock = BaseClock_Core2,
	.TurboClock = NULL,
	.Architecture = "Core2/Conroe/Yonah",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_NONE,
	.PCI_ids = PCI_Core2_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},
/*  5*/	{
	.Signature = _Core_Yorkfield,
	.Query = Query_Core2,
	.Update = PerCore_Core2_Query,
	.Start = Start_Core2,
	.Stop = Stop_Core2,
	.Exit = NULL,
	.Timer = InitTimer_Core2,
	.BaseClock = BaseClock_Core2,
	.TurboClock = NULL,
	.Architecture = "Core2/Yorkfield",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_NONE,
	.PCI_ids = PCI_Core2_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},
/*  6*/	{
	.Signature = _Core_Dunnington,
	.Query = Query_Core2,
	.Update = PerCore_Core2_Query,
	.Start = Start_Core2,
	.Stop = Stop_Core2,
	.Exit = NULL,
	.Timer = InitTimer_Core2,
	.BaseClock = BaseClock_Core2,
	.TurboClock = NULL,
	.Architecture = "Xeon/Dunnington",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_NONE,
	.PCI_ids = PCI_Core2_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},

/*  7*/	{
	.Signature = _Atom_Bonnell,
	.Query = Query_Core2,
	.Update = PerCore_Core2_Query,
	.Start = Start_Core2,
	.Stop = Stop_Core2,
	.Exit = NULL,
	.Timer = InitTimer_Core2,
	.BaseClock = BaseClock_Atom,
	.TurboClock = NULL,
	.Architecture = "Atom/Bonnell",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_NONE,
	.PCI_ids = PCI_Void_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},
/*  8*/	{
	.Signature = _Atom_Silvermont,
	.Query = Query_Core2,
	.Update = PerCore_Core2_Query,
	.Start = Start_Core2,
	.Stop = Stop_Core2,
	.Exit = NULL,
	.Timer = InitTimer_Core2,
	.BaseClock = BaseClock_Atom,
	.TurboClock = NULL,
	.Architecture = "Atom/Silvermont",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_NONE,
	.PCI_ids = PCI_Void_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},
/*  9*/	{
	.Signature = _Atom_Lincroft,
	.Query = Query_Core2,
	.Update = PerCore_Core2_Query,
	.Start = Start_Core2,
	.Stop = Stop_Core2,
	.Exit = NULL,
	.Timer = InitTimer_Core2,
	.BaseClock = BaseClock_Atom,
	.TurboClock = NULL,
	.Architecture = "Atom/Lincroft",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_NONE,
	.PCI_ids = PCI_Void_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},
/* 10*/	{
	.Signature = _Atom_Clovertrail,
	.Query = Query_Core2,
	.Update = PerCore_Core2_Query,
	.Start = Start_Core2,
	.Stop = Stop_Core2,
	.Exit = NULL,
	.Timer = InitTimer_Core2,
	.BaseClock = BaseClock_Atom,
	.TurboClock = NULL,
	.Architecture = "Atom/Clovertrail",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_NONE,
	.PCI_ids = PCI_Void_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},
/* 11*/	{
	.Signature = _Atom_Saltwell,
	.Query = Query_Core2,
	.Update = PerCore_Core2_Query,
	.Start = Start_Core2,
	.Stop = Stop_Core2,
	.Exit = NULL,
	.Timer = InitTimer_Core2,
	.BaseClock = BaseClock_Atom,
	.TurboClock = NULL,
	.Architecture = "Atom/Saltwell",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_NONE,
	.PCI_ids = PCI_Void_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},

/* 12*/	{
	.Signature = _Silvermont_637,
	.Query = Query_Core2,
	.Update = PerCore_Core2_Query,
	.Start = Start_Core2,
	.Stop = Stop_Core2,
	.Exit = NULL,
	.Timer = InitTimer_Core2,
	.BaseClock = BaseClock_Silvermont,
	.TurboClock = NULL,
	.Architecture = "Silvermont",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_INTEL_ATOM,
	.PCI_ids = PCI_Void_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},
/* 13*/	{
	.Signature = _Atom_Avoton,
	.Query = Query_Nehalem,
	.Update = PerCore_Nehalem_Query,
	.Start = Start_Nehalem,
	.Stop = Stop_Nehalem,
	.Exit = NULL,
	.Timer = InitTimer_Nehalem,
	.BaseClock = BaseClock_Silvermont,
	.TurboClock = Intel_Turbo_Config8C,
	.Architecture = "Atom/Avoton",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_INTEL_ATOM,
	.PCI_ids = PCI_Void_ids,
	.Uncore = {
		.Start = Start_Uncore_Nehalem,
		.Stop = Stop_Uncore_Nehalem
		},
	.Specific = Void_Specific
	},

/* 14*/	{
	.Signature = _Atom_Airmont,
	.Query = Query_Core2,
	.Update = PerCore_Core2_Query,
	.Start = Start_Core2,
	.Stop = Stop_Core2,
	.Exit = NULL,
	.Timer = InitTimer_Core2,
	.BaseClock = BaseClock_Airmont,
	.TurboClock = NULL,
	.Architecture = "Atom/Airmont",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_NONE,
	.PCI_ids = PCI_Void_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},
/* 15*/	{
	.Signature = _Atom_Goldmont,
	.Query = Query_SandyBridge,
	.Update = PerCore_Haswell_ULT_Query,
	.Start = Start_Haswell_ULT,
	.Stop = Stop_Haswell_ULT,
	.Exit = NULL,
	.Timer = InitTimer_Haswell_ULT,
	.BaseClock = BaseClock_Haswell,
	.TurboClock = Intel_Turbo_Config8C,
	.Architecture = "Atom/Goldmont",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_NONE,
	.PCI_ids = PCI_Void_ids,
	.Uncore = {
		.Start = Start_Uncore_Haswell_ULT,
		.Stop = Stop_Uncore_Haswell_ULT
		},
	.Specific = Void_Specific
	},
/* 16*/	{
	.Signature = _Atom_Sofia,
	.Query = Query_Core2,
	.Update = PerCore_Core2_Query,
	.Start = Start_Core2,
	.Stop = Stop_Core2,
	.Exit = NULL,
	.Timer = InitTimer_Core2,
	.BaseClock = BaseClock_Atom,
	.TurboClock = NULL,
	.Architecture = "Atom/Sofia",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_INTEL_ATOM,
	.PCI_ids = PCI_Void_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},
/* 17*/	{
	.Signature = _Atom_Merrifield,
	.Query = Query_Core2,
	.Update = PerCore_Core2_Query,
	.Start = Start_Core2,
	.Stop = Stop_Core2,
	.Exit = NULL,
	.Timer = InitTimer_Core2,
	.BaseClock = BaseClock_Atom,
	.TurboClock = NULL,
	.Architecture = "Atom/Merrifield",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_INTEL_ATOM,
	.PCI_ids = PCI_Void_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},
/* 18*/	{
	.Signature = _Atom_Moorefield,
	.Query = Query_Core2,
	.Update = PerCore_Core2_Query,
	.Start = Start_Core2,
	.Stop = Stop_Core2,
	.Exit = NULL,
	.Timer = InitTimer_Core2,
	.BaseClock = BaseClock_Atom,
	.TurboClock = NULL,
	.Architecture = "Atom/Moorefield",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_INTEL_ATOM,
	.PCI_ids = PCI_Void_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},

/* 19*/	{
	.Signature = _Nehalem_Bloomfield,
	.Query = Query_Nehalem,
	.Update = PerCore_Nehalem_Query,
	.Start = Start_Nehalem,
	.Stop = Stop_Nehalem,
	.Exit = NULL,
	.Timer = InitTimer_Nehalem,
	.BaseClock = BaseClock_Nehalem,
	.TurboClock = Intel_Turbo_Config8C,
	.Architecture = "Nehalem/Bloomfield",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_NONE,
	.PCI_ids = PCI_Nehalem_QPI_ids,
	.Uncore = {
		.Start = Start_Uncore_Nehalem,
		.Stop = Stop_Uncore_Nehalem
		},
	.Specific = Nehalem_Bloomfield_Specific
	},
/* 20*/	{
	.Signature = _Nehalem_Lynnfield,
	.Query = Query_Nehalem,
	.Update = PerCore_Nehalem_Query,
	.Start = Start_Nehalem,
	.Stop = Stop_Nehalem,
	.Exit = NULL,
	.Timer = InitTimer_Nehalem,
	.BaseClock = BaseClock_Nehalem,
	.TurboClock = Intel_Turbo_Config8C,
	.Architecture = "Nehalem/Lynnfield",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_NONE,
	.PCI_ids = PCI_Nehalem_DMI_ids,
	.Uncore = {
		.Start = Start_Uncore_Nehalem,
		.Stop = Stop_Uncore_Nehalem
		},
	.Specific = Void_Specific
	},
/* 21*/	{
	.Signature = _Nehalem_MB,
	.Query = Query_Nehalem,
	.Update = PerCore_Nehalem_Query,
	.Start = Start_Nehalem,
	.Stop = Stop_Nehalem,
	.Exit = NULL,
	.Timer = InitTimer_Nehalem,
	.BaseClock = BaseClock_Nehalem,
	.TurboClock = Intel_Turbo_Config8C,
	.Architecture = "Nehalem/Mobile",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_NONE,
	.PCI_ids = PCI_Nehalem_DMI_ids,
	.Uncore = {
		.Start = Start_Uncore_Nehalem,
		.Stop = Stop_Uncore_Nehalem
		},
	.Specific = Void_Specific
	},
/* 22*/	{
	.Signature = _Nehalem_EX,
	.Query = Query_Core2,
	.Update = PerCore_Nehalem_Query,
	.Start = Start_Nehalem,
	.Stop = Stop_Nehalem,
	.Exit = NULL,
	.Timer = InitTimer_Nehalem,
	.BaseClock = BaseClock_Nehalem,
	.TurboClock = NULL,
	.Architecture = "Nehalem/eXtreme.EP",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_NONE,
	.PCI_ids = PCI_Nehalem_QPI_ids,
	.Uncore = {
		.Start = Start_Uncore_Nehalem,
		.Stop = Stop_Uncore_Nehalem
		},
	.Specific = Void_Specific
	},

/* 23*/	{
	.Signature = _Westmere,
	.Query = Query_Nehalem,
	.Update = PerCore_Nehalem_Query,
	.Start = Start_Nehalem,
	.Stop = Stop_Nehalem,
	.Exit = NULL,
	.Timer = InitTimer_Nehalem,
	.BaseClock = BaseClock_Westmere,
	.TurboClock = Intel_Turbo_Config8C,
	.Architecture = "Westmere",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_NONE,
	.PCI_ids = PCI_Nehalem_DMI_ids,
	.Uncore = {
		.Start = Start_Uncore_Nehalem,
		.Stop = Stop_Uncore_Nehalem
		},
	.Specific = Void_Specific
	},
/* 24*/	{
	.Signature = _Westmere_EP,
	.Query = Query_Nehalem,
	.Update = PerCore_Nehalem_Query,
	.Start = Start_Nehalem,
	.Stop = Stop_Nehalem,
	.Exit = NULL,
	.Timer = InitTimer_Nehalem,
	.BaseClock = BaseClock_Westmere,
	.TurboClock = Intel_Turbo_Config8C,
	.Architecture = "Westmere/EP",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_NONE,
	.PCI_ids = PCI_Westmere_EP_ids,
	.Uncore = {
		.Start = Start_Uncore_Nehalem,
		.Stop = Stop_Uncore_Nehalem
		},
	.Specific = Void_Specific
	},
/* 25*/	{
	.Signature = _Westmere_EX,
	.Query = Query_Core2,
	.Update = PerCore_Nehalem_Query,
	.Start = Start_Nehalem,
	.Stop = Stop_Nehalem,
	.Exit = NULL,
	.Timer = InitTimer_Nehalem,
	.BaseClock = BaseClock_Westmere,
	.TurboClock = NULL,
	.Architecture = "Westmere/eXtreme",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_NONE,
	.PCI_ids = PCI_Nehalem_QPI_ids,
	.Uncore = {
		.Start = Start_Uncore_Nehalem,
		.Stop = Stop_Uncore_Nehalem
		},
	.Specific = Void_Specific
	},

/* 26*/	{
	.Signature = _SandyBridge,
	.Query = Query_SandyBridge,
	.Update = PerCore_SandyBridge_Query,
	.Start = Start_SandyBridge,
	.Stop = Stop_SandyBridge,
	.Exit = NULL,
	.Timer = InitTimer_SandyBridge,
	.BaseClock = BaseClock_SandyBridge,
	.TurboClock = Intel_Turbo_Config8C,
	.Architecture = "SandyBridge",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_INTEL_SNB,
	.powerFormula   = POWER_FORMULA_INTEL,
	.PCI_ids = PCI_SandyBridge_ids,
	.Uncore = {
		.Start = Start_Uncore_SandyBridge,
		.Stop = Stop_Uncore_SandyBridge
		},
	.Specific = Void_Specific
	},
/* 27*/	{
	.Signature = _SandyBridge_EP,
	.Query = Query_SandyBridge,
	.Update = PerCore_SandyBridge_EP_Query,
	.Start = Start_SandyBridge_EP,
	.Stop = Stop_SandyBridge_EP,
	.Exit = NULL,
	.Timer = InitTimer_SandyBridge_EP,
	.BaseClock = BaseClock_SandyBridge,
	.TurboClock = Intel_Turbo_Config8C,
	.Architecture = "SandyBridge/eXtreme.EP",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_INTEL_SNB,
	.powerFormula   = POWER_FORMULA_INTEL,
	.PCI_ids = PCI_SandyBridge_ids,
	.Uncore = {
		.Start = Start_Uncore_SandyBridge_EP,
		.Stop = Stop_Uncore_SandyBridge_EP
		},
	.Specific = Void_Specific
	},

/* 28*/	{
	.Signature = _IvyBridge,
	.Query = Query_IvyBridge,
	.Update = PerCore_IvyBridge_Query,
	.Start = Start_SandyBridge,
	.Stop = Stop_SandyBridge,
	.Exit = NULL,
	.Timer = InitTimer_SandyBridge,
	.BaseClock = BaseClock_IvyBridge,
	.TurboClock = Intel_Turbo_Config8C,
	.Architecture = "IvyBridge",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_INTEL_SNB,
	.powerFormula   = POWER_FORMULA_INTEL,
	.PCI_ids = PCI_IvyBridge_ids,
	.Uncore = {
		.Start = Start_Uncore_SandyBridge,
		.Stop = Stop_Uncore_SandyBridge
		},
	.Specific = Void_Specific
	},
/* 29*/	{
	.Signature = _IvyBridge_EP,
	.Query = Query_IvyBridge_EP,
	.Update = PerCore_IvyBridge_EP_Query,
	.Start = Start_SandyBridge_EP,
	.Stop = Stop_SandyBridge_EP,
	.Exit = NULL,
	.Timer = InitTimer_SandyBridge_EP,
	.BaseClock = BaseClock_IvyBridge,
	.TurboClock = TurboClock_IvyBridge_EP,
	.Architecture = "IvyBridge/EP",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_INTEL_SNB,
	.powerFormula   = POWER_FORMULA_INTEL,
	.PCI_ids = PCI_IvyBridge_ids,
	.Uncore = {
		.Start = Start_Uncore_SandyBridge_EP,
		.Stop = Stop_Uncore_SandyBridge_EP
		},
	.Specific = Void_Specific
	},

/* 30*/	{
	.Signature = _Haswell_DT,
	.Query = Query_Haswell,
	.Update = PerCore_IvyBridge_Query,
	.Start = Start_SandyBridge,
	.Stop = Stop_SandyBridge,
	.Exit = NULL,
	.Timer = InitTimer_SandyBridge,
	.BaseClock = BaseClock_Haswell,
	.TurboClock = Intel_Turbo_Config8C,
	.Architecture = "Haswell/Desktop",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_INTEL_SNB,
	.powerFormula   = POWER_FORMULA_INTEL,
	.PCI_ids = PCI_Haswell_ids,
	.Uncore = {
		.Start = Start_Uncore_SandyBridge,
		.Stop = Stop_Uncore_SandyBridge
		},
	.Specific = Void_Specific
	},
/* 31*/	{
	.Signature = _Haswell_EP,
	.Query = Query_Haswell_EP,
	.Update = PerCore_Haswell_EP_Query,
	.Start = Start_Haswell_EP,
	.Stop = Stop_Haswell_EP,
	.Exit = NULL,
	.Timer = InitTimer_Haswell_EP,
	.BaseClock = BaseClock_Haswell,
	.TurboClock = TurboClock_Haswell_EP,
	.Architecture = "Haswell/EP/Mobile",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_INTEL_SNB,
	.powerFormula   = POWER_FORMULA_INTEL,
	.PCI_ids = PCI_Haswell_ids,
	.Uncore = {
		.Start = Start_Uncore_Haswell_EP,
		.Stop = Stop_Uncore_Haswell_EP
		},
	.Specific = Void_Specific
	},
/* 32*/	{
	.Signature = _Haswell_ULT,
	.Query = Query_IvyBridge,
	.Update = PerCore_Haswell_ULT_Query,
	.Start = Start_Haswell_ULT,
	.Stop = Stop_Haswell_ULT,
	.Exit = NULL,
	.Timer = InitTimer_Haswell_ULT,
	.BaseClock = BaseClock_Haswell,
	.TurboClock = Intel_Turbo_Config8C,
	.Architecture = "Haswell/Ultra Low TDP",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_INTEL_SNB,
	.powerFormula   = POWER_FORMULA_INTEL,
	.PCI_ids = PCI_Haswell_ids,
	.Uncore = {
		.Start = Start_Uncore_Haswell_ULT,
		.Stop = Stop_Uncore_Haswell_ULT
		},
	.Specific = Void_Specific
	},
/* 33*/	{
	.Signature = _Haswell_ULX,
	.Query = Query_IvyBridge,
	.Update = PerCore_IvyBridge_Query,
	.Start = Start_SandyBridge,
	.Stop = Stop_SandyBridge,
	.Exit = NULL,
	.Timer = InitTimer_SandyBridge,
	.BaseClock = BaseClock_Haswell,
	.TurboClock = Intel_Turbo_Config8C,
	.Architecture = "Haswell/Ultra Low eXtreme",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_INTEL_SNB,
	.powerFormula   = POWER_FORMULA_INTEL,
	.PCI_ids = PCI_Haswell_ids,
	.Uncore = {
		.Start = Start_Uncore_SandyBridge,
		.Stop = Stop_Uncore_SandyBridge
		},
	.Specific = Void_Specific
	},

/* 34*/	{
	.Signature = _Broadwell,
	.Query = Query_Broadwell,
	.Update = PerCore_Broadwell_Query,
	.Start = Start_Broadwell,
	.Stop = Stop_Broadwell,
	.Exit = NULL,
	.Timer = InitTimer_Broadwell,
	.BaseClock = BaseClock_Haswell,
	.TurboClock = Intel_Turbo_Config8C,
	.Architecture = "Broadwell/Mobile",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_INTEL_SNB,
	.powerFormula   = POWER_FORMULA_INTEL,
	.PCI_ids = PCI_Broadwell_ids,
	.Uncore = {
		.Start = Start_Uncore_Broadwell,
		.Stop = Stop_Uncore_Broadwell
		},
	.Specific = Void_Specific
	},
/* 35*/	{
	.Signature = _Broadwell_D,
	.Query = Query_Haswell_EP,
	.Update = PerCore_Haswell_EP_Query,
	.Start = Start_Haswell_EP,
	.Stop = Stop_Haswell_EP,
	.Exit = NULL,
	.Timer = InitTimer_Haswell_EP,
	.BaseClock = BaseClock_Haswell,
	.TurboClock = TurboClock_Haswell_EP,
	.Architecture = "Broadwell/D",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_INTEL_SNB,
	.powerFormula   = POWER_FORMULA_INTEL,
	.PCI_ids = PCI_Broadwell_ids,
	.Uncore = {
		.Start = Start_Uncore_Haswell_EP,
		.Stop = Stop_Uncore_Haswell_EP
		},
	.Specific = Void_Specific
	},
/* 36*/	{
	.Signature = _Broadwell_H,
	.Query = Query_Broadwell,
	.Update = PerCore_Broadwell_Query,
	.Start = Start_Broadwell,
	.Stop = Stop_Broadwell,
	.Exit = NULL,
	.Timer = InitTimer_Broadwell,
	.BaseClock = BaseClock_Haswell,
	.TurboClock = Intel_Turbo_Config8C,
	.Architecture = "Broadwell/H",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_INTEL_SNB,
	.powerFormula   = POWER_FORMULA_INTEL,
	.PCI_ids = PCI_Broadwell_ids,
	.Uncore = {
		.Start = Start_Uncore_Broadwell,
		.Stop = Stop_Uncore_Broadwell
		},
	.Specific = Void_Specific
	},
/* 37*/	{
	.Signature = _Broadwell_EP,
	.Query = Query_Haswell_EP,
	.Update = PerCore_Haswell_EP_Query,
	.Start = Start_Haswell_EP,
	.Stop = Stop_Haswell_EP,
	.Exit = NULL,
	.Timer = InitTimer_Haswell_EP,
	.BaseClock = BaseClock_Haswell,
	.TurboClock = TurboClock_Haswell_EP,
	.Architecture = "Broadwell/EP/EX",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_INTEL_SNB,
	.powerFormula   = POWER_FORMULA_INTEL,
	.PCI_ids = PCI_Broadwell_ids,
	.Uncore = {
		.Start = Start_Uncore_Haswell_EP,
		.Stop = Stop_Uncore_Haswell_EP
		},
	.Specific = Void_Specific
	},

/* 38*/	{
	.Signature = _Skylake_UY,
	.Query = Query_Broadwell,
	.Update = PerCore_Skylake_Query,
	.Start = Start_Skylake,
	.Stop = Stop_Skylake,
	.Exit = NULL,
	.Timer = InitTimer_Skylake,
	.BaseClock = BaseClock_Skylake,
	.TurboClock = Intel_Turbo_Config8C,
	.Architecture = "Skylake/UY",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_INTEL_SNB,
	.powerFormula   = POWER_FORMULA_INTEL,
	.PCI_ids = PCI_Skylake_ids,
	.Uncore = {
		.Start = Start_Uncore_Skylake,
		.Stop = Stop_Uncore_Skylake
		},
	.Specific = Void_Specific
	},
/* 39*/	{
	.Signature = _Skylake_S,
	.Query = Query_Broadwell,
	.Update = PerCore_Skylake_Query,
	.Start = Start_Skylake,
	.Stop = Stop_Skylake,
	.Exit = NULL,
	.Timer = InitTimer_Skylake,
	.BaseClock = BaseClock_Skylake,
	.TurboClock = Intel_Turbo_Config8C,
	.Architecture = "Skylake/S",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_INTEL_SNB,
	.powerFormula   = POWER_FORMULA_INTEL,
	.PCI_ids = PCI_Skylake_ids,
	.Uncore = {
		.Start = Start_Uncore_Skylake,
		.Stop = Stop_Uncore_Skylake
		},
	.Specific = Void_Specific
	},
/* 40*/	{
	.Signature = _Skylake_X,
	.Query = Query_Skylake_X,
	.Update = PerCore_Skylake_X_Query,
	.Start = Start_Skylake_X,
	.Stop = Stop_Skylake_X,
	.Exit = NULL,
	.Timer = InitTimer_Skylake_X,
	.BaseClock = BaseClock_Skylake,
	.TurboClock = TurboClock_Skylake_X,
	.Architecture = "Skylake/X",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_INTEL_SKL_X,
	.powerFormula   = POWER_FORMULA_INTEL,
	.PCI_ids = PCI_Skylake_X_ids,
	.Uncore = {
		.Start = Start_Uncore_Skylake_X,
		.Stop = Stop_Uncore_Skylake_X
		},
	.Specific = Void_Specific
	},

/* 41*/	{
	.Signature = _Xeon_Phi,
	.Query = Query_SandyBridge,
	.Update = PerCore_SandyBridge_Query,
	.Start = Start_SandyBridge,
	.Stop = Stop_SandyBridge,
	.Exit = NULL,
	.Timer = InitTimer_SandyBridge,
	.BaseClock = BaseClock_Skylake,
	.TurboClock = NULL,
	.Architecture = "Knights Landing",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_INTEL,
	.PCI_ids = PCI_Void_ids,
	.Uncore = {
		.Start = Start_Uncore_SandyBridge,
		.Stop = Stop_Uncore_SandyBridge
		},
	.Specific = Void_Specific
	},

/* 42*/	{
	.Signature = _Kabylake,
	.Query = Query_Broadwell,
	.Update = PerCore_Skylake_Query,
	.Start = Start_Skylake,
	.Stop = Stop_Skylake,
	.Exit = NULL,
	.Timer = InitTimer_Skylake,
	.BaseClock = BaseClock_Skylake,
	.TurboClock = Intel_Turbo_Config8C,
	.Architecture = "Kaby/Coffee Lake",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_INTEL_SNB,
	.powerFormula   = POWER_FORMULA_INTEL,
	.PCI_ids = PCI_Kabylake_ids,
	.Uncore = {
		.Start = Start_Uncore_Skylake,
		.Stop = Stop_Uncore_Skylake
		},
	.Specific = Kabylake_Specific
	},
/* 43*/	{
	.Signature = _Kabylake_UY,
	.Query = Query_Broadwell,
	.Update = PerCore_Skylake_Query,
	.Start = Start_Skylake,
	.Stop = Stop_Skylake,
	.Exit = NULL,
	.Timer = InitTimer_Skylake,
	.BaseClock = BaseClock_Skylake,
	.TurboClock = Intel_Turbo_Config8C,
	.Architecture = "Kaby Lake/UY",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_INTEL_SNB,
	.powerFormula   = POWER_FORMULA_INTEL,
	.PCI_ids = PCI_Kabylake_ids,
	.Uncore = {
		.Start = Start_Uncore_Skylake,
		.Stop = Stop_Uncore_Skylake
		},
	.Specific = Void_Specific
	},

/* 44*/	{
	.Signature = _Cannonlake,
	.Query = Query_Broadwell,
	.Update = PerCore_Skylake_Query,
	.Start = Start_Skylake,
	.Stop = Stop_Skylake,
	.Exit = NULL,
	.Timer = InitTimer_Skylake,
	.BaseClock = BaseClock_Skylake,
	.TurboClock = Intel_Turbo_Config8C,
	.Architecture = "Cannon Lake",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_INTEL_SNB,
	.powerFormula   = POWER_FORMULA_INTEL,
	.PCI_ids = PCI_Void_ids,
	.Uncore = {
		.Start = Start_Uncore_Skylake,
		.Stop = Stop_Uncore_Skylake
		},
	.Specific = Void_Specific
	},

/* 45*/	{
	.Signature = _Geminilake,
	.Query = Query_SandyBridge,
	.Update = PerCore_Haswell_ULT_Query,
	.Start = Start_Haswell_ULT,
	.Stop = Stop_Haswell_ULT,
	.Exit = NULL,
	.Timer = InitTimer_Haswell_ULT,
	.BaseClock = BaseClock_Haswell,
	.TurboClock = Intel_Turbo_Config8C,
	.Architecture = "Atom/Gemini Lake",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_NONE,
	.powerFormula   = POWER_FORMULA_NONE,
	.PCI_ids = PCI_Void_ids,
	.Uncore = {
		.Start = Start_Uncore_Haswell_ULT,
		.Stop = Stop_Uncore_Haswell_ULT
		},
	.Specific = Void_Specific
	},

/* 46*/	{
	.Signature = _Icelake_UY,
	.Query = Query_SandyBridge,
	.Update = PerCore_Skylake_Query,
	.Start = Start_Skylake,
	.Stop = Stop_Skylake,
	.Exit = NULL,
	.Timer = InitTimer_Skylake,
	.BaseClock = BaseClock_Skylake,
	.TurboClock = Intel_Turbo_Config8C,
	.Architecture = "Ice Lake/UY",
	.thermalFormula = THERMAL_FORMULA_INTEL,
	.voltageFormula = VOLTAGE_FORMULA_INTEL_SNB,
	.powerFormula   = POWER_FORMULA_INTEL,
	.PCI_ids = PCI_Void_ids,
	.Uncore = {
		.Start = Start_Uncore_Skylake,
		.Stop = Stop_Uncore_Skylake
		},
	.Specific = Void_Specific
	},

	{
	.Signature = _AMD_Family_0Fh,
	.Query = Query_AMD_Family_0Fh,
	.Update = PerCore_AMD_Family_0Fh_Query,
	.Start = Start_AMD_Family_0Fh,
	.Stop = Stop_AMD_Family_0Fh,
	.Exit = NULL,
	.Timer = InitTimer_AMD_Family_0Fh,
	.BaseClock = BaseClock_AuthenticAMD,
	.TurboClock = NULL,
	.Architecture = "Family 0Fh",
	.thermalFormula = THERMAL_FORMULA_AMD_0Fh,
	.voltageFormula = VOLTAGE_FORMULA_AMD_0Fh,
	.powerFormula   = POWER_FORMULA_AMD,
	.PCI_ids = PCI_AMD_0Fh_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},

	{
	.Signature = _AMD_Family_10h,
	.Query = Query_AMD_Family_10h,
	.Update = PerCore_AMD_Family_10h_Query,
	.Start = Start_AMD_Family_10h,
	.Stop = Stop_AMD_Family_10h,
	.Exit = NULL,
	.Timer = InitTimer_AuthenticAMD,
	.BaseClock = BaseClock_AuthenticAMD,
	.TurboClock = NULL,
	.Architecture = "Family 10h",
	.thermalFormula = THERMAL_FORMULA_AMD,
	.voltageFormula = VOLTAGE_FORMULA_AMD,
	.powerFormula   = POWER_FORMULA_AMD,
	.PCI_ids = PCI_Void_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},

	{
	.Signature = _AMD_Family_11h,
	.Query = Query_AMD_Family_11h,
	.Update = PerCore_AMD_Family_11h_Query,
	.Start = Start_AMD_Family_11h,
	.Stop = Stop_AMD_Family_11h,
	.Exit = NULL,
	.Timer = InitTimer_AuthenticAMD,
	.BaseClock = BaseClock_AuthenticAMD,
	.TurboClock = NULL,
	.Architecture = "Family 11h",
	.thermalFormula = THERMAL_FORMULA_AMD,
	.voltageFormula = VOLTAGE_FORMULA_AMD,
	.powerFormula   = POWER_FORMULA_AMD,
	.PCI_ids = PCI_Void_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},

	{
	.Signature = _AMD_Family_12h,
	.Query = Query_AMD_Family_12h,
	.Update = PerCore_AMD_Family_12h_Query,
	.Start = Start_AuthenticAMD,
	.Stop = Stop_AuthenticAMD,
	.Exit = NULL,
	.Timer = InitTimer_AuthenticAMD,
	.BaseClock = BaseClock_AuthenticAMD,
	.TurboClock = NULL,
	.Architecture = "Family 12h",
	.thermalFormula = THERMAL_FORMULA_AMD,
	.voltageFormula = VOLTAGE_FORMULA_AMD,
	.powerFormula   = POWER_FORMULA_AMD,
	.PCI_ids = PCI_Void_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},

	{
	.Signature = _AMD_Family_14h,
	.Query = Query_AMD_Family_14h,
	.Update = PerCore_AMD_Family_14h_Query,
	.Start = Start_AuthenticAMD,
	.Stop = Stop_AuthenticAMD,
	.Exit = NULL,
	.Timer = InitTimer_AuthenticAMD,
	.BaseClock = BaseClock_AuthenticAMD,
	.TurboClock = NULL,
	.Architecture = "Family 14h",
	.thermalFormula = THERMAL_FORMULA_AMD,
	.voltageFormula = VOLTAGE_FORMULA_AMD,
	.powerFormula   = POWER_FORMULA_AMD,
	.PCI_ids = PCI_Void_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},

	{
	.Signature = _AMD_Family_15h,
	.Query = Query_AMD_Family_15h,
	.Update = PerCore_AMD_Family_15h_Query,
	.Start = Start_AuthenticAMD,
	.Stop = Stop_AuthenticAMD,
	.Exit = NULL,
	.Timer = InitTimer_AuthenticAMD,
	.BaseClock = BaseClock_AuthenticAMD,
	.TurboClock = NULL,
	.Architecture = "Family 15h",
	.thermalFormula = THERMAL_FORMULA_AMD,
	.voltageFormula = VOLTAGE_FORMULA_AMD,
	.powerFormula   = POWER_FORMULA_AMD,
	.PCI_ids = PCI_Void_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},

	{
	.Signature = _AMD_Family_16h,
	.Query = Query_AMD_Family_15h,
	.Update = PerCore_AMD_Family_15h_Query,
	.Start = Start_AuthenticAMD,
	.Stop = Stop_AuthenticAMD,
	.Exit = NULL,
	.Timer = InitTimer_AuthenticAMD,
	.BaseClock = BaseClock_AuthenticAMD,
	.TurboClock = NULL,
	.Architecture = "Family 16h",
	.thermalFormula = THERMAL_FORMULA_AMD,
	.voltageFormula = VOLTAGE_FORMULA_AMD,
	.powerFormula   = POWER_FORMULA_AMD,
	.PCI_ids = PCI_Void_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},

	{
	.Signature = _AMD_Family_17h,
	.Query = Query_AMD_Family_17h,
	.Update = PerCore_AMD_Family_17h_Query,
	.Start = Start_AMD_Family_17h,
	.Stop = Stop_AMD_Family_17h,
	.Exit = NULL,
	.Timer = InitTimer_AMD_Family_17h,
	.BaseClock = BaseClock_AMD_Family_17h,
	.TurboClock = NULL,
	.Architecture = "Family 17h",
	.thermalFormula = THERMAL_FORMULA_AMD_17h,
	.voltageFormula = VOLTAGE_FORMULA_AMD_17h,
	.powerFormula   = POWER_FORMULA_AMD_17h,
	.PCI_ids = PCI_AMD_17h_ids,
	.Uncore = {
		.Start = NULL,
		.Stop = NULL
		},
	.Specific = Void_Specific
	},
};

static struct ZEN_ST {
	char *brandSubStr;
	unsigned int Boost, XFR;	/* Round +0.5 XFR to +1 */
	THERMAL_PARAM Param;		/* Source: Linux/k10temp.c */
} Zen_Table[] = {
	{VENDOR_AMD,	/* Default */	+0,  0, .Param.Offset = { 0, 0}},
	{"AMD Ryzen 3 1200",		+3, +1, .Param.Offset = { 0, 0}},
	{"AMD Ryzen 3 1300X",		+2, +2, .Param.Offset = { 0, 0}},
	{"AMD Ryzen 3 2200G",		+2,  0, .Param.Offset = { 0, 0}},
	{"AMD Ryzen 5 1400",		+2, +1, .Param.Offset = { 0, 0}},
	{"AMD Ryzen 5 2400G",		+3,  0, .Param.Offset = { 0, 0}},
	{"AMD Ryzen 5 1500X",		+2, +2, .Param.Offset = { 0, 0}},
	{"AMD Ryzen 5 2500U",		+16, 0, .Param.Offset = { 0, 0}},
	{"AMD Ryzen 5 1600X",		+4, +1, .Param.Offset = {20, 0}},
	{"AMD Ryzen 5 1600",		+4, +1, .Param.Offset = { 0, 0}},
	{"AMD Ryzen 5 2600X",		+5, +2, .Param.Offset = {10, 0}},
	{"AMD Ryzen 5 2600",		+3, +2, .Param.Offset = { 0, 0}},
	{"AMD Ryzen 7 1700X",		+4, +1, .Param.Offset = {20, 0}},
	{"AMD Ryzen 7 1700",		+7, +1, .Param.Offset = { 0, 0}},
	{"AMD Ryzen 7 1800X",		+4, +1, .Param.Offset = {20, 0}},
	{"AMD Ryzen 7 2700X",		+5, +2, .Param.Offset = {10,49}},
	{"AMD Ryzen 7 2700U",		+16, 0, .Param.Offset = { 0, 0}},
	{"AMD Ryzen 7 2700",		+8, +2, .Param.Offset = { 0, 0}},
	{"AMD Ryzen Threadripper 1950X",+6, +2, .Param.Offset = {27, 0}},
	{"AMD Ryzen Threadripper 1920X",+5, +2, .Param.Offset = {27, 0}},
	{"AMD Ryzen Threadripper 1900X",+2, +2, .Param.Offset = {27, 0}},
	{"AMD Ryzen Threadripper 2990", +12, 0, .Param.Offset = {27, 0}},
	{"AMD Ryzen Threadripper 2970", +12, 0, .Param.Offset = {27, 0}},
	{"AMD Ryzen Threadripper 2950", +9,  0, .Param.Offset = {27, 0}},
	{"AMD Ryzen Threadripper 2920", +8,  0, .Param.Offset = {27, 0}},
	{"AMD EPYC 7351P",		+5,  0, .Param.Offset = { 0, 0}},
	{"AMD EPYC 7401P",		+8, +2, .Param.Offset = { 0, 0}},
	{"AMD EPYC 7551P",		+6, +4, .Param.Offset = { 0, 0}},
	{"AMD EPYC 7251",		+8,  0, .Param.Offset = { 0, 0}},
	{"AMD EPYC 7281",		+6,  0, .Param.Offset = { 0, 0}},
	{"AMD EPYC 7301",		+5,  0, .Param.Offset = { 0, 0}},
	{"AMD EPYC 7351",		+5,  0, .Param.Offset = { 0, 0}},
	{"AMD EPYC 7401",		+8, +2, .Param.Offset = { 0, 0}},
	{"AMD EPYC 7451",		+6, +3, .Param.Offset = { 0, 0}},
	{"AMD EPYC 7501",		+6, +4, .Param.Offset = { 0, 0}},
	{"AMD EPYC 7551",		+6, +4, .Param.Offset = { 0, 0}},
	{"AMD EPYC 7601",		+5, +5, .Param.Offset = { 0, 0}}
};
