/*
 * CS460: Operating Systems
 * Jim Plank
 * fperror.c -- Showing floating point errors with the lwp library
 */


#include <unistd.h>
#include <stdlib.h>
#include <lwp/lwp.h>
#include <lwp/check.h>
#include <lwp/lwpmachdep.h>
#include <lwp/stackdep.h>
#include <stdio.h>


/* Each thread repeats calculating f2 and f3, which should equal
   each other.  If the two do not equal each other, it prints
   out their values.   */

thread(n)
int n;
{
  double f1, f2, f3;
  char s[300];
  int iteration;

  iteration = 0;
  f1 = (double) n;

  while(1) {

    iteration++;
    f2 = f1*f1 * (f1 + 1.0) - 2.0*(f1+5.0);

    f3 = f1* (- 2.0 + f1*f1 + f1) - 10.0;
  
    if (f2 != f3) {
      sprintf(s, "Thread %d, iteration %d: f2 != f3.  f2=%lf  f3=%lf\n", 
              n, iteration, f2, f3);
      write(1, s, strlen(s));
    }
  }

}


/* The main thread forks off 5 "thread" threads, and preempts them */

main(argc, argv)
int argc; 
char **argv;
{
  int i;
  stkalign_t *stacks[5];
  thread_t threads[5];
  struct timeval t;

  if (argc != 3) {
    fprintf(stderr, "usage: fperror timersec timerusec\n");
    exit(1);
  }

  t.tv_sec = atoi(argv[1]);
  t.tv_usec = atoi(argv[2]);

  pod_setmaxpri(2);

  lwp_setstkcache(MINSTACKSZ*sizeof(stkalign_t), 5);

  for (i = 0; i < 5; i++) {
    stacks[i] = lwp_newstk();
    lwp_create(threads+i, thread, 1, 0, stacks[i], 1, i);
  }

  while(1) {
    lwp_sleep(&t);
    lwp_resched(1);
  }
}
