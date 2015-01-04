/*
 * CS170: Operating Systems
 *	Rich Wolski
 * 	modified from CS460 Operating Systems -- Jim Plank
 * dphil_skeleton.c -- Dining philosophers driver program
 */


#include <stdio.h>
#include <pthread.h>
#include "dphil.h"


void *philosopher(void *v)
{
  Phil_struct *ps;
  long st;
  long t;

  ps = (Phil_struct *) v;
  while(1) {

    /* First the philosopher thinks for a random number of seconds */

    st = (random()%ps->ms) + 1;
    printf("%3d Philosopher %d thinking for %d seconds\n", 
                time(0)-ps->t0, ps->id, st);
    fflush(stdout);
    sleep(st);

    /* Now, the philosopher wakes up and wants to eat.  He calls pickup
       to pick up the chopsticks */

    printf("%3d Philosopher %d no longer thinking -- calling pickup()\n", 
            time(0)-ps->t0, ps->id);
    fflush(stdout);
    t = time(0);
    pickup(ps);
    pthread_mutex_lock(ps->blockmon);
    ps->blocktime[ps->id] += (time(0) - t);
    pthread_mutex_unlock(ps->blockmon);

    /* When pickup returns, the philosopher can eat for a random number of
       seconds */

    st = (random()%ps->ms) + 1;
    printf("%3d Philosopher %d eating for %d seconds\n", 
                time(0)-ps->t0, ps->id, st);
    fflush(stdout);
    sleep(st);

    /* Finally, the philosopher is done eating, and calls putdown to
       put down the chopsticks */

    printf("%3d Philosopher %d no longer eating -- calling putdown()\n", 
            time(0)-ps->t0, ps->id);
    fflush(stdout);
    putdown(ps);
  }
}

main(argc, argv)
int argc; 
char **argv;
{
  int i;
  pthread_t threads[MAXTHREADS];
  Phil_struct ps[MAXTHREADS];
  void *v;
  long t0;
  pthread_mutex_t *blockmon;
  int *blocktime;
  char s[500];
  int phil_count;
  char *curr;
  int total;

  if (argc != 3) {
    fprintf(stderr, "usage: dphil philosopher_count maxsleepsec\n");
    exit(1);
  }

  srandom(time(0));
  
  phil_count = atoi(argv[1]);
  
  if(phil_count > MAXTHREADS)
	  phil_count = MAXTHREADS;
   
  v = initialize_state(phil_count);
  t0 = time(0);
  blocktime = (int *) malloc(sizeof(int)*phil_count);
  blockmon = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(blockmon, NULL);
  for (i = 0; i < phil_count; i++) blocktime[i] = 0;

  for (i = 0; i < phil_count; i++) {
    ps[i].id = i;
    ps[i].t0 = t0;
    ps[i].v = v;
    ps[i].ms = atoi(argv[2]);
    ps[i].blocktime = blocktime;
    ps[i].blockmon = blockmon;
    pthread_create(threads+i, NULL, philosopher, (void *) (ps+i));
  }

  while(1) {
    pthread_mutex_lock(blockmon);
    curr = s;
    total=0;
    for(i=0; i < phil_count; i++)
	    total += blocktime[i];
    sprintf(curr,"%3d Total blocktime: %5d : ", 
		    time(0)-t0,
		    total);
    curr = s + strlen(s);
    for(i=0; i < phil_count; i++)
    {
    	sprintf(curr, "%5d ", blocktime[i]);
	curr = s + strlen(s);
    }
    pthread_mutex_unlock(blockmon);
    printf("%s\n", s);
    fflush(stdout);
    sleep(10);
  }
}
