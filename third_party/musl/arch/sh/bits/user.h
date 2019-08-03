#undef __WORDSIZE
#define __WORDSIZE 32

#define REG_REG0	 0
#define REG_REG15	15
#define REG_PC		16
#define REG_PR		17
#define REG_SR		18
#define REG_GBR		19
#define REG_MACH	20
#define REG_MACL	21
#define REG_SYSCALL	22
#define REG_FPREG0	23
#define REG_FPREG15	38
#define REG_XFREG0	39
#define REG_XFREG15	54
#define REG_FPSCR	55
#define REG_FPUL	56

struct pt_regs {
	unsigned long regs[16];
	unsigned long pc;
	unsigned long pr;
	unsigned long sr;
	unsigned long gbr;
	unsigned long mach;
	unsigned long macl;
	long tra;
};

struct pt_dspregs {
	unsigned long a1;
	unsigned long a0g;
	unsigned long a1g;
	unsigned long m0;
	unsigned long m1;
	unsigned long a0;
	unsigned long x0;
	unsigned long x1;
	unsigned long y0;
	unsigned long y1;
	unsigned long dsr;
	unsigned long rs;
	unsigned long re;
	unsigned long mod;
};

struct user_fpu_struct {
	unsigned long fp_regs[16];
	unsigned long xfp_regs[16];
	unsigned long fpscr;
	unsigned long fpul;
};

#define ELF_NGREG 23
typedef unsigned long elf_greg_t;
typedef elf_greg_t elf_gregset_t[ELF_NGREG];
typedef struct user_fpu_struct elf_fpregset_t;

struct user {
	struct pt_regs regs;
	struct user_fpu_struct fpu;
	int u_fpvalid;
	unsigned long u_tsize;
	unsigned long u_dsize;
	unsigned long u_ssize;
	unsigned long start_code;
	unsigned long start_data;
	unsigned long start_stack;
	long int signal;
	unsigned long u_ar0;
	struct user_fpu_struct *u_fpstate;
	unsigned long magic;
	char u_comm[32];
};
