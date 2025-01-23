/*
 * CS170: Operating Systems
 * Rich Wolski 
 * Jim Plank
 * dphil_8.c -- Dining philosophers solution #8 -- the book's solution
 * plus aging -- i.e. do the algorithm of solution #7 whenever your
 * neighbors have gone ms*5 seconds without eating (and they're hungry).
 */


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "dphil.h"


#define THINKING 0
#define HUNGRY 1
#define EATING 2

typedef struct {
  pthread_mutex_t *mon;
  pthread_cond_t *cv[MAXTHREADS];
  int state[MAXTHREADS];
  int tblock[MAXTHREADS];     /* The is the time when the philosopher first blocks
                        in pickup */
  int phil_count;
} Phil;


int can_I_eat(Phil *pp, int n, int ms)
{
  long t;
  int phil_count;
  
  phil_count = pp->phil_count;

  /*
   * if my left or right neighbor is eating...
   */
  if (pp->state[(n+(phil_count-1))%phil_count] == EATING ||
      pp->state[(n+1)%phil_count] == EATING) 
		return(0);

  /*
   * if my left neighbor is hungry, became hungry before me, and has been
   * waiting longer than ms*5...
   */
  t = time(0);
  if (pp->state[(n+(phil_count-1))%phil_count] == HUNGRY &&
      pp->tblock[(n+(phil_count-1))%phil_count] < pp->tblock[n] &&
      (t - pp->tblock[(n+(phil_count-1))%phil_count]) >= ms*5) 
		return(0);

  /*
   * if my right neighbor is hungry, became hungry before me, and has been
   * waiting longer than ms*5...
   */
  if (pp->state[(n+1)%phil_count] == HUNGRY &&
      pp->tblock[(n+1)%phil_count] < pp->tblock[n] &&
      (t - pp->tblock[(n+1)%phil_count]) >= ms*5) 
		return(0);

  return(1);
}

void pickup(Phil_struct *ps)
{
  Phil *pp;

  pp = (Phil *) ps->v;
  
  pthread_mutex_lock(pp->mon);
  pp->state[ps->id] = HUNGRY;
  pp->tblock[ps->id] = time(0);
  while (!can_I_eat(pp, ps->id, ps->ms)) {
    pthread_cond_wait(pp->cv[ps->id], pp->mon);
  }
  pp->state[ps->id] = EATING;
  pthread_mutex_unlock(pp->mon);
}

void putdown(Phil_struct *ps)
{
  Phil *pp;
  int phil_count;

  pp = (Phil *) ps->v;
  phil_count = pp->phil_count;

  pthread_mutex_lock(pp->mon);
  pp->state[ps->id] = THINKING;
  if (pp->state[(ps->id+(phil_count-1))%phil_count] == HUNGRY) {
    pthread_cond_signal(pp->cv[(ps->id+(phil_count-1))%phil_count]);
  }
  if (pp->state[(ps->id+1)%phil_count] == HUNGRY) {
    pthread_cond_signal(pp->cv[(ps->id+1)%phil_count]);
  }
  pthread_mutex_unlock(pp->mon);
}

void *initialize_state(int phil_count)
{
  Phil *pp;
  int i;

  pp = (Phil *) malloc(sizeof(Phil));
  pp->phil_count = phil_count;
  pp->mon = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(pp->mon, NULL);
  for (i = 0; i < phil_count; i++) {
    pp->cv[i] = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
    pthread_cond_init(pp->cv[i], NULL);
    pp->state[i] = THINKING;
    pp->tblock[i] = 0;
  }

  return (void *) pp;
}
