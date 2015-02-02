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
  int eats[MAXTHREADS];
  int max;
  int phil_count;
} Phil;


int can_I_eat(Phil *pp, int n)
{
  long t;
  int phil_count;
  int left_eat;
  int right_eat;
  int my_eat;
  
  phil_count = pp->phil_count;

  if (pp->state[(n+(phil_count-1))%phil_count] == EATING ||
      pp->state[(n+1)%phil_count] == EATING) 
		return(0);

  my_eat = pp->eats[n];
  left_eat = pp->eats[(n+(phil_count-1))%phil_count];
  right_eat = pp->eats[(n+1)%phil_count];

printf("t: %d, me: %d, left: %d, right: %d max: %d\n",n,my_eat,left_eat,right_eat,pp->max);

/*
  if(my_eat < pp->max)
  {
printf("%d returnng 1\n",n);
	return(1);
  }
*/

  if ((pp->state[(n+(phil_count-1))%phil_count] == HUNGRY) &&
      	(left_eat < pp->max))
  {
printf("%d returnng 0\n",n);
		return(0);
  }

  if ((pp->state[(n+1)%phil_count] == HUNGRY) &&
      (right_eat < pp->max))
  {
printf("%d returnng 0\n",n);
		return(0);
  }

printf("%d returnng 1\n",n);
  return(1);
}

void pickup(Phil_struct *ps)
{
  Phil *pp;
  int now;

  pp = (Phil *) ps->v;
  
  pthread_mutex_lock(pp->mon);
  pp->state[ps->id] = HUNGRY;
  while (!can_I_eat(pp, ps->id)) {
    pthread_cond_wait(pp->cv[ps->id], pp->mon);
  }
  pp->eats[ps->id] += 1;
  if(pp->eats[ps->id] > pp->max)
  {
	pp->max = pp->eats[ps->id];
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

  pp = (Phil *) malloc(sizeof(Phil)*phil_count);
  pp->phil_count = phil_count;
  pp->mon = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
  pp->max = 0;
  pthread_mutex_init(pp->mon, NULL);
  for (i = 0; i < phil_count; i++) {
    pp->cv[i] = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
    pthread_cond_init(pp->cv[i], NULL);
    pp->state[i] = THINKING;
    pp->eats[i] = 0;
  }

  return (void *) pp;
}
