#if !defined(KT_H)
#define KT_H

/*
 * returns my thread id
 */
void *kt_self();


/*
 * kt_fork() creates a new, runnable thread
 *
 * func: entry point for the new thread
 * arg: single argument to be passed
 */
void *kt_fork(void *(*func)(void *), void *arg);

/*
 * kt_join() blocks waiting for the completion of the thread specified
 *           in its argument list
 *
 * kt: the thread to wait for
 */
void kt_join(void *kt);

/*
 * kt_joinall() blocks until there are not runnable threads left
 */
void kt_joinall();

/*
 * kt_exit() kills the calling thread immediately
 */
void kt_exit();

/*
 **** Semaphore entry points start here
 */
typedef void *kt_sem;

/*
 * make_kt_sem() creates and initializes with its first argument which must
 *               be >= 0
 *               
 * initval: initial semaphore value
 */
kt_sem make_kt_sem(int initval);

/*
 * kill_kt_sem() deallocates the semaphore.  If there are threads blocked on
 *               the semaphore, kill_kt_sem() will cause the program to exit.
 *
 * ksem: semaphore to deallocate
 */
void kill_kt_sem(kt_sem ksem);

/*
 * kt_getval(kt_sem s) retursn the current value
 */

int kt_getval(kt_sem s);

/*
 * P_kt_sem() performs the P semaphore operation.  The current value is
 *            decremented and if it is < 0, the calling thread is queued on
 *            the semaphore and blocked
 *
 * ksem: relevant semaphore
 */
void P_kt_sem(kt_sem ksem);

/*
 * V_kt_sem() performs the V operation.  The value is incremented.  If it is 
 *            < 0, a thread is dequeued and awakened.
 *
 * ksem: relevant semaphore
 */
void V_kt_sem(kt_sem ksem);

/*
 * kt_sleep(t) puts the thread to sleep for t seconds
 */
void kt_sleep(int secs);

/*
 * kt_yield() abdicates the CPU to that other runnable threads may execute
 */
void kt_yield();

/*
 * kt_kill() allows one thread to kill another
 */
void kt_kill(void *t);

#include <stdint.h>
extern uintptr_t __pointer_chk_guard_local();
#endif

