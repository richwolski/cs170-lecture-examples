#include "kos_world.h"

#include <time.h>

/* kt.c -- KThreads library 
   Rich Wolski
   Jim Plank
   CS560 -- Operating Systems
   Department of Computer Science, University of Tennessee
   Wed Jan 31 10:25:57 EST 2001

   This is a nice non-preemptive threads library for
   CS560.  This runs on Solaris and Linux at present.  See 
   http://www.cs.utk.edu/~plank/plank/classes/cs560/560/notes/KThreads1/lecture.html 
   for lecture notes on how to use the library.

   See:
   http://www.cs.utk.edu/~plank/plank/classes/cs560/560/notes/KThreads2/lecture.html 
   for lecture notes on the implementation.

 */


#ifdef __svr4__
#ifndef SOLARIS
#define SOLARIS
#endif
#endif

#ifdef __linux__
#ifndef LINUX
#define LINUX
#endif
#endif

#include <setjmp.h>

#ifdef LINUX

#if defined(__x86_64__)
/*
 * maintain 32-bit compat with 64-bit; define a type representing
 * register size. don't use size_t / ssize_t because type names won't
 * play nice with the jrb_* functions
 */
#define REGTYPE        long

/* from glibc v2.37 source code, file: sysdeps/x86_64/jmpbuf-offsets.h */
#define JB_RBX	0
#define JB_BP	1 /* changed from JB_RBP for compat */
#define JB_R12	2
#define JB_R13	3
#define JB_R14	4
#define JB_R15	5
#define JB_SP	6 /* changed from JB_RSP for compat */
#define JB_PC	7
#define JB_SIZE (8*8)

#elif defined(__i386__)
#define REGTYPE        int
#ifndef JB_BP /* in libc >= 3.5 they hide JB_* and mangle SP */
#define	JB_BP 3
#define	JB_SP 4
#endif
#endif

#include <features.h> /* header file defining the GLIBC version */
#if (__GLIBC__ >= 2 && __GLIBC_MINOR__ >= 8)
/* the new mangling method (as of 2.8 (or even 2.7?) */
#pragma message("Using the new pointer mangling method (glibc 2.8 and up)")

#ifdef __i386__
#define PTR_MANGLE(var)       asm ("xorl %%gs:%c2, %0\n"	\
					 "roll $9, %0"\
					 : "=r" (var) \
                                         : "0" (var), \
					   "i" (24))
#elif defined(__x86_64__)
//#include <stdint.h>
//extern __attribute__ ((section (".data.rel.ro"))) uintptr_t __pointer_chk_guard_local;

#define LP_SIZE "8"
#define POINTER_GUARD           (48) /* offsetof (tcbhead_t, pointer_guard),
                                      * see sysdeps/x86_64/nptl/tls.h and
                                      * sysdeps/x86_64/nptl/tls.h
                                      */
/* used outside of the glibc RTLD (dynamic linker) module.
 * see sysdeps/unix/sysv/linux/x86_64/pointer_guard.h */
#  define PTR_MANGLE(var)       asm ("xor %%fs:%c2, %0\n"                     \
                                     "rol $2*" LP_SIZE "+1, %0"               \
                                     : "=r" (var)                             \
                                     : "0" (var),                             \
                                       "i" (POINTER_GUARD))

#endif /* ifdef __i386__ */
#else /* the original pointer mangling method */
#warning Using the old pointer mangling method (up to glibc 2.7)
#define PTR_MANGLE(var)       asm ("xorl %%gs:%c2, %0"\
					 : "=r" (var) \
                                         : "0" (var), \
					   "i" (24))
#endif
#else /* JB_BP and JB_SP are available and no mangling needed */
#warning No pointer mangling is necessary with this glibc
#define PTR_MANGLE(var)       /* no-op */
#endif

#ifdef SOLARIS
#define JB_SP (1)
#define JB_FP (3)
#endif

#ifndef __CONCAT /* newer GCC defines this for us in <cdefs.h> */
#define __CONCAT(s1, s2)        s1 ## s2
#endif
#define CONCAT(s1, s2)          __CONCAT(s1, s2)

#define jrb_insert_regt         CONCAT(jrb_insert_, REGTYPE)
#define jrb_find_regt           CONCAT(jrb_find_, REGTYPE)

#include "kdebug.h"
#include "kt.h"

#define KT_STACK_SIZE (16*1024)

#define STARTING (0)
#define RUNNING (1)
#define RUNNABLE (2)
#define BLOCKED (3)
#define SLEEPING (4)
#define DEAD (5)

typedef struct kt_sem_str *Ksem;
struct kt_sem_str
{
	int val;		/* The value */
        REGTYPE sid;		/* Unique id */
};

struct kt_str
{
	void *(*func)(void *);	/* function to call */
	void *arg;		/* arg to pass that function */
        REGTYPE tid;                /* Unique thread id */
	int state;		/* queue state */
	JRB blocked_list;	/* list I'm blocked on */
	JRB blocked_list_ptr;	/* pointer to my node on the list */
	jmp_buf jmpbuf;		/* stack/PC state */
	jmp_buf exitbuf;	/* stack/PC state for immediate exit */
	char *stack;		/* stack pointer */
	ssize_t stack_size;	/* stack_size */
	unsigned REGTYPE wake_time; /* if I'm sleeping, should I wake now? */
	int die_now;
	Ksem ks;		/* in case I get killed */
};


typedef struct kt_str *K_t;

static K_t ktRunning;			/* running thread */
static Dllist ktFree_me;		/* threads waiting to be freed */
static Dllist ktRunnable;		/* run queue */
static JRB ktBlocked;			/* waiting for thread action */
static JRB ktSleeping;			/* waiting for Godot */
static JRB ktActive;			/* searchable list of active threads */

static size_t ktThread_count;
static REGTYPE ktTidCounter;
static REGTYPE ktSidCounter;

static REGTYPE KtInit_d = 0;

static K_t ktOriginal;			/* global for main thread */


  
K_t InitKThread(ssize_t stack_size, void *(*func)(void *), void *arg)
{
	K_t kt;
	void *stack = NULL;

	if(stack_size > 0)
	{
		stack = (char *)malloc(stack_size);
		memset(stack,0,stack_size);
	}

	if((stack_size > 0) && (stack == NULL))
	{
		if(Debug & KT_DEBUG)
		{
			fprintf(stderr,"InitKThread: no space for stack");
			fflush(stderr);
			return(NULL);
		}
	}

	kt = (K_t)malloc(sizeof(*kt));
	if(kt == NULL)
	{
		if(stack != NULL)
			free(stack);
		if(Debug & KT_DEBUG)
		{
			fprintf(stderr,"InitKThread: no space for struct\n");
			fflush(stderr);
		}
		return(NULL);
	}

    	kt->tid = ktTidCounter++;
	kt->stack = stack;
	kt->stack_size = stack_size;
	kt->func = func;
	kt->arg = arg;
	kt->state = 0;
	kt->die_now = 0;

	ktThread_count++;

	/*
	 * keep track of all threads int he system
	 */
	jrb_insert_regt(ktActive,kt->tid,new_jval_v(kt));

	return(kt);
}

void
InitKThreadSystem()
{
	int ok;

	if(KtInit_d)
	{
		return;
	}

	ok = 0;

#ifdef SOLARIS
	ok++;
#endif

#ifdef LINUX
	ok++;
#endif

	if (ok == 0) {
          fprintf(stderr, "KThread error: No architecture specified %s\n",
                          "at compile time");
          exit(1);
        }
        if (ok > 1) {
          fprintf(stderr, "KThread error: Multiple architectures %s\n",
                          "specified at compile time");
          exit(1);
        }

	ktActive = make_jrb();
	ktRunnable = new_dllist();
	ktFree_me = new_dllist();
	ktBlocked = make_jrb();
	ktSleeping = make_jrb();
        ktThread_count = 0;
	ktTidCounter = 1;
	ktSidCounter = -1;

	ktOriginal = InitKThread(0,NULL,NULL);
	ktRunning = ktOriginal;

	KtInit_d = 1;

	return;
}



void
FreeKThread(K_t kt)
{
	JRB krb;

	if(kt == ktOriginal)
	{
		return;
	}

	free(kt->stack);
	free(kt);
	ktThread_count--;
	return;
}

void
WakeKThread(K_t kt)
{
	/*
	 * look through the various blocked lists and try to wake the
	 * specified thread
	 */

        if (kt->state == RUNNING || kt->state == RUNNABLE 
                                 || kt->state == DEAD) return;

        jrb_delete_node(kt->blocked_list_ptr);
	kt->state = RUNNABLE;
	kt->blocked_list = NULL;
	kt->blocked_list_ptr = NULL;
	dll_append(ktRunnable,new_jval_v(kt));
	return;
}

void
BlockKThread(K_t kt, REGTYPE key)
{
	kt->state = BLOCKED;
	kt->blocked_list = ktBlocked;
	kt->blocked_list_ptr = jrb_insert_regt(ktBlocked,key,new_jval_v(kt));
	return;
}


void
SleepKThread(K_t kt, REGTYPE until)
{
	kt->state = SLEEPING;
	kt->blocked_list = ktSleeping;
	kt->wake_time = until;
	kt->blocked_list_ptr = jrb_insert_regt(ktSleeping,until,new_jval_v(kt));
	return;
}

void
FreeFinishedThreads()
{
	Dllist curr;

        while (!dll_empty(ktFree_me)) 
        {
                curr = dll_first(ktFree_me);
		FreeKThread((K_t)curr->val.v);
		dll_delete_node(curr);
        }
        return;
}

void
KtSched()
{
	K_t kt;
	JRB jb;
	unsigned REGTYPE sp;
	unsigned REGTYPE now;
        JRB tmp;
        Dllist dtmp;

	/*
	 * start by recording the current stack contents in case
	 * I'm descheduled
	 *
	 * this is where I return to when I'm rescheduled
	 */
	if(setjmp(ktRunning->jmpbuf) != 0)
	{
		FreeFinishedThreads();
		/*
		 * if we are being killed by another thread, jump through
		 * the exitbuf
		 */
		if(ktRunning->die_now)
		{
                        /* Jim: This used to longjmp to the exitbuf,
                                but I changed it for two reasons:  
                                1. It wasn't being removed from ktActive
                                2. Hell will be paid if it is ktOriginal.
                                I believe kt_exit() is cleaner.  I
                                have not tested it.  I should.  */
			kt_exit();
			/* not reached */
		}
		return;
	}


start:

        if (!jrb_empty(ktSleeping)) {
  	  now = time(0);
	  while(!jrb_empty(ktSleeping))
	  {
	  	kt = (K_t) jval_v(jrb_val(jrb_first(ktSleeping)));
		if(kt->wake_time > now)
		{
			break;
		}
		WakeKThread(kt);
          }
	}
			


	/*
	 * if there is nothing left to run, exit.  However, if there 
         * are sleepers or a joinall, deal with them appropriately
	 */
	if(dll_empty(ktRunnable))
	{

		/*
		 * first, check for sleepers and deal with them
		 */
		if(!jrb_empty(ktSleeping))
		{
			kt = jval_v(jrb_val(jrb_first(ktSleeping)));
			sleep(kt->wake_time - now);
			goto start;
		}

		/*
		 * next, see if there is a joinall thread waiting
		 */
		jb = jrb_find_regt(ktBlocked,0);
		if(jb != NULL)
		{
			WakeKThread((K_t)jval_v(jrb_val(jb)));
			goto start;
		}

		if(!jrb_empty(ktBlocked)) 
		{
			if(Debug & KT_DEBUG)
			{
				fprintf(stderr,
					"All processes blocked, exiting\n");
				fflush(stderr);
			}
			exit(1);
		} else {
			if(Debug & KT_DEBUG)
			{
				fprintf(stderr,
					"No runnable threads, exiting\n");
				fflush(stderr);
			}
			exit(0);
                }

                fprintf(stderr, "We shouldn't get here\n");
                exit(1);
	}

        /* Grab the first job of the ready queue */

        dtmp = dll_first(ktRunnable);
        kt = (K_t) dtmp->val.v;
	dll_delete_node(dtmp);

        /* If it is runnable, run it */

	if(kt->state == RUNNABLE) {

		ktRunning = kt;
		ktRunning->state = RUNNING;
		longjmp(ktRunning->jmpbuf,1);
                /* This doesn't return */
	}

	/*
	 * if we have never run before, set up initial stack and go
	 */
	if(kt->state == STARTING) {
		if(setjmp(kt->jmpbuf) == 0)
		{
			/*
			 * get double word aligned SP -- stacks grow from high
			 * to low
			 */
			sp = (unsigned REGTYPE)&((kt->stack[kt->stack_size - 1]));
			while((sp % (2 * sizeof(REGTYPE))) != 0)
				sp--;
#ifdef LINUX
			/*
			 * keep double word aligned but put in enough
			 * space to handle local variables for KtSched
			 */
			kt->jmpbuf->__jmpbuf[JB_BP] = (REGTYPE)sp;
			kt->jmpbuf->__jmpbuf[JB_SP] = (REGTYPE)sp-1024;
			PTR_MANGLE(kt->jmpbuf->__jmpbuf[JB_SP]);
                        PTR_MANGLE(kt->jmpbuf->__jmpbuf[JB_BP]);
#endif

#ifdef SOLARIS
			/*
			 * keep double word aligned but put in enough
			 * space to handle local variables for KtSched
			 */
			kt->jmpbuf[JB_FP] = (int)sp;
			kt->jmpbuf[JB_SP] = (int)sp-1024;
#endif
			/*
			 * set ktRunning while we still have local variables
			 */
			kt->state = RUNNING;
			ktRunning = kt;
			/*
			 * now jump onto the new stack
			 */
			longjmp(kt->jmpbuf,1);
		}
		else
		{
			/*
			 * here we are on a new, clean stack -- touch nothing,
			 * set the state, and call
			 *
			 * ktRunning is global so there is no local variable
			 * problem
			 *
			 * borrow this stack to try and free the last thread
			 * if there was one
			 */

			FreeFinishedThreads();

			if(setjmp(ktRunning->exitbuf) == 0)
			{
				/*
				 * if we were killed before we ran, skip the
				 * function call
				 */
				if(ktRunning->die_now == 0)
				{
					ktRunning->func(ktRunning->arg);
				}
			}


			/*
			 * we are back and this thread is done
			 *
			 * make it inactive
			 */

			jb = jrb_find_regt(ktActive,ktRunning->tid);
			if(jb == NULL)
			{
				if(Debug & KT_DEBUG)
				{
					fprintf(stderr,
				"KtSched: panic -- inactive return\n");
					fflush(stderr);
				}
				exit(1);
			}
			jrb_delete_node(jb);

			/*
			 * look to see if there is a thread waiting for this
			 * one to exit -- careful with locals
			 */
			jb = jrb_find_regt(ktBlocked,ktRunning->tid);
			if(jb != NULL)
			{
				WakeKThread((K_t)jval_v(jrb_val(jb)));
			}


			/*
			 * all we can do now is to commit suicide
			 *
			 * don't touch the locals;
			 *
			 * and don't free the stack we are running on
			 */
			FreeFinishedThreads();
                        ktRunning->state = DEAD;
			dll_append(ktFree_me,new_jval_v(ktRunning));
			ktRunning = NULL;
			goto start;
		}
	}

        /* The only way we get here is if there was a thread on the
           runnable queue whose state was not RUNNABLE or STARTING.
           Flag that as an error */

        fprintf(stderr, 
                "Error: non-STARTING or RUNNABLE thread on the ready queue\n");
        exit(1);
}


/*
 * fork a thread and make it runnable
 */
void *
kt_fork(void *(*func)(void *), void *arg)
{
	K_t kt;

	InitKThreadSystem();

	kt = InitKThread(KT_STACK_SIZE,func,arg);

	if(kt == NULL)
	{
		if(Debug & KT_DEBUG)
		{
			fprintf(stderr,"kt_fork: couldn't make thread\n");
			fflush(stderr);
		}
		return(NULL);
	}

	kt->state = STARTING;
	dll_append(ktRunnable,new_jval_v(kt));
	return((void *) (kt->tid));
}

/*
 * block and wait for the thread (passed as i_join) to complete
 */
void
kt_join(void *i_join)
{
	K_t me;
	JRB target;
        REGTYPE tid;

	InitKThreadSystem();

        tid = (REGTYPE) i_join;

        if (tid <= 0) {
          fprintf(stderr, "kt_join() -- bad argument\n");
          exit(1);
        }

	/*
	 * see if the thread I want to wait for exists
	 */

	target = jrb_find_regt(ktActive,tid);

	/*
	 * if not, we assume that the thread is dead, and simply return.
         * Unfortunately, that might not be right, but it's the best we
         * can do without burning too much memory.
	 */
	if(target == NULL) return;

        if (jrb_find_regt(ktBlocked, tid) != NULL) {
          fprintf(stderr, "Called kt_join on a thread twice\n");
          exit(1);
        }

	BlockKThread(ktRunning,tid);

	KtSched();

	return;
}

void
kt_joinall()
{
	K_t me;
	JRB jb;

	InitKThreadSystem();


        /* Jim: I'm changing semantics.  If there is already
           a joinall thread, flag it as an error */

	if(jrb_find_regt(ktBlocked,0) != NULL)
	{
          fprintf(stderr, "Error: two joinall threads\n");
          exit(1);
	}
	/*
	 * 0 is the sleep key for joinall
	 */
	BlockKThread(ktRunning,0);

	/*
	 * cause the scheduler to block
	 */
	KtSched();

	return;
}

void *kt_self()
{
	InitKThreadSystem();

	return((void *) (ktRunning->tid));
}

void kt_exit()
{
        JRB tmp;

	InitKThreadSystem();
	/*
	 * unroll stack and escape
	 */
	if(Debug & KT_DEBUG)
	{
		printf("longjmping to 0x%x exitbuf\n",ktRunning->func);
	}
 
        /* Now, do one of two things.  If this is the main (original)
           thread, you simply want to set its state to dead, and 
           reschedule.  You don't want to clean up its stack, because
           its stack is the main stack.  

           If this is any other thread, then longjmp to its exitbuf.
           This jumps you down to the bottom of the thread's stack, and
           the thread will put itself on the ktFree_me list, so that it
           can be cleaned up */
    
        if (ktRunning == ktOriginal) {
          ktRunning->state = DEAD;

          /* Nuke it from ktActive */
          tmp = jrb_find_regt(ktActive, ktRunning->tid);
          if (tmp == NULL) {
            fprintf(stderr, "Panic: Original thread not in ktActive\n");
            exit(1);
          }
          jrb_delete_node(tmp);

          /* If there is a thread waiting on me, wake it up */

          tmp = jrb_find_regt(ktBlocked, ktRunning->tid);
          if (tmp != NULL) {
            WakeKThread((K_t)tmp->val.v);
          }

          KtSched();

          /* This should never return, because this thread will never
             be rescheduled */
          fprintf(stderr, 
                  "Error: main thread returned after calling kt_exit()\n");
          exit(1);
        } else {
	  longjmp(ktRunning->exitbuf,1);
        }
}

void *
make_kt_sem(int initval)
{
	Ksem ks;

	InitKThreadSystem();

	if(initval < 0)
	{
		if(Debug & KT_DEBUG)
		{
			fprintf(stderr,"make_kt_sem: bad initial val: %d\n",
					initval);
			fflush(stderr);
		}
		return(NULL);
	}

	ks = (Ksem)malloc(sizeof(*ks));
	if(ks == NULL)
	{
		if(Debug & KT_DEBUG)
		{
			fprintf(stderr,"make_kt_sem: no space\n");
			fflush(stderr);
		}
		return(NULL);
	}

	ks->val = initval;
	ks->sid = ktSidCounter--;

	return((void *)ks);
}

void kill_kt_sem(kt_sem iks)
{
	Ksem ks = (Ksem)iks;

	InitKThreadSystem();

	/*
	 * if there are threads blocked on this semaphore, panic
	 */
	if(jrb_find_regt(ktBlocked,ks->sid) != NULL)
	{
		if(Debug & KT_DEBUG)
		{
			fprintf(stderr,
	"kill_kt_sem: deletion with waiting threads, val: %d\n",
				ks->val);
			fflush(stderr);
		}
		exit(1);
	}

	free(ks);

	return;
}

void P_kt_sem(kt_sem iks)
{
	Ksem ks = (Ksem)iks;
	K_t me = ktRunning;

	InitKThreadSystem();

	ks->val--;

	if(ks->val < 0)
	{
		/*
		 * use the semaphore tid as the blocking key
		 */
		ktRunning->ks = ks;
		BlockKThread(ktRunning,ks->sid);
		KtSched();
		ktRunning->ks = NULL;
		return;
	}

	return;
}

void V_kt_sem(kt_sem iks)
{
	Ksem ks = (Ksem)iks;
	K_t wake_kt;

	InitKThreadSystem();

	ks->val++;

	if(ks->val <= 0)
	{
		wake_kt = jval_v(jrb_val(jrb_find_regt(ktBlocked,ks->sid)));
		WakeKThread(wake_kt);
	}

	return;
}

void
kt_sleep(int secs)
{
	int until = time(0)+secs;

	InitKThreadSystem();

	SleepKThread(ktRunning,until);
	KtSched();
	return;
}

void kt_yield()
{
	InitKThreadSystem();

	ktRunning->state = RUNNABLE;
	dll_append(ktRunnable,new_jval_v(ktRunning));
	KtSched();
	return;
}

int kt_getval(kt_sem s)
{
	Ksem k;

	InitKThreadSystem();

	k = (Ksem)s;

	return(k->val);
}

void kt_kill(void *t)
{
	K_t kt;
        REGTYPE tid;
        JRB tmp;

        tid = (REGTYPE) t;

        tmp = jrb_find_regt(ktActive, tid);

        /* Hell, this might not be right either.  If the thread
           is not in the active tree, then assume it's dead */

        if (tmp == NULL) return;

	kt = (K_t) tmp->val.v;

	kt->die_now = 1;

	if((kt->state == BLOCKED) || (kt->state == SLEEPING))
	{
		/*
		 * if I'm blocked on a semaphore and I get killed,
		 * better bump the semaphore count
		 */
		if(kt->ks != NULL)
		{
			kt->ks->val++;
		}
		WakeKThread(kt);
	}

	return;
}




