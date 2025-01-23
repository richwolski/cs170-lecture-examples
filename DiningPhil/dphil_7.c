/*
 * CS460: Operating Systems
 * Jim Plank
 * dphil_7.c -- Dining philosophers solution #7 -- a better solution 
 * with no starvation
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
  int queuenum[MAXTHREADS];
  int qn;
  int phil_count;
} Phil;


int can_I_eat(Phil *pp, int n)
{
  int phil_count;
  int i_can_eat;
  
  phil_count = pp->phil_count;
  i_can_eat = 1;

  /*
   * if either neighbor is eating...
   */
  if (pp->state[(n+(phil_count-1))%phil_count] == EATING ||
      pp->state[(n+1)%phil_count] == EATING) 
		i_can_eat = 0;

  /*
   * if my left neighbor is hungry and has a lower ticket than I
   */
  if (pp->state[(n+(phil_count-1))%phil_count] == HUNGRY &&
      pp->queuenum[(n+(phil_count-1))%phil_count] < pp->queuenum[n]) 
		i_can_eat = 0;

  /*
   * if my right neighbor is hungry and has a lower ticket than I
   */
  if (pp->state[(n+1)%phil_count] == HUNGRY &&
      pp->queuenum[(n+1)%phil_count] < pp->queuenum[n]) 
		i_can_eat = 0;
  
  return(i_can_eat);
}

static char stc[3] = {'T', 'H', 'E'};

void pickup(Phil_struct *ps)
{
  Phil *pp;

  pp = (Phil *) ps->v;
  
  
  pthread_mutex_lock(pp->mon);
  pp->state[ps->id] = HUNGRY;
  pp->queuenum[ps->id] = pp->qn++;
  while (!can_I_eat(pp, ps->id)) {
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
  pthread_cond_signal(pp->cv[(ps->id+(phil_count-1))%phil_count]);
  pthread_cond_signal(pp->cv[(ps->id+1)%phil_count]);
  pthread_mutex_unlock(pp->mon);
}

void *initialize_state(int phil_count)
{
  Phil *pp;
  int i;


  pp = (Phil *) malloc(sizeof(Phil));
  pp->phil_count = phil_count;
  pp->mon = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
  pp->qn = 0;
  pthread_mutex_init(pp->mon, NULL);
  for (i = 0; i < phil_count; i++) {
    pp->cv[i] = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
    pthread_cond_init(pp->cv[i], NULL);
    pp->state[i] = THINKING;
    pp->queuenum[i] = 0;
  }

  return (void *) pp;
}
