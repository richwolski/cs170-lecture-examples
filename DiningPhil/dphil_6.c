/*
 * CS170: Operating Systems
 * 	adapted from CS460: Operating Systems
 * 	Jim Plank
 * dphil_6.c -- Dining philosophers solution
 * a starvation-free solution using a global queue.
 */


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "dphil.h"
#include "dllist.h"
#include "jval.h"

#define THINKING 0
#define HUNGRY 1
#define EATING 2

typedef struct {
  pthread_mutex_t *mon;
  pthread_cond_t *cv[MAXTHREADS];
  int state[MAXTHREADS];
  Dllist q;
  int phil_count;
} Phil;


/* It is assumed that you have locked pp->mon */

test_queue(void *v)
{
  int id;
  int phil_count;
  Phil *pp = (Phil *)v;
  
  phil_count = pp->phil_count;

  if (!dll_empty(pp->q)) {
    id = (int) jval_i(dll_val(dll_first(pp->q)));
    if (pp->state[(id+1)%phil_count] != EATING && 
		    pp->state[(id+(phil_count-1))%phil_count] != EATING) {
      pthread_cond_signal(pp->cv[id]);
    }
  }
}
    
/* If the queue is empty and your neighbors are not eating, then
   go ahead and eat.  Otherwise, put yourself on the queue and
   wait */

void pickup(Phil_struct *ps)
{
  Phil *pp;
  int phil_count;
  Dllist node;

  pp = (Phil *) ps->v;
  phil_count = pp->phil_count;
  
  pthread_mutex_lock(pp->mon);
  if (dll_empty(pp->q) && 
		  pp->state[((ps->id+1))%phil_count] != EATING &&
                     pp->state[(ps->id+(phil_count-1))%phil_count] != EATING) {
    pp->state[ps->id] = EATING;
  } else {
    /*
     * technically, we don't need to retest because each Phil has its own
     * cond variable and signal must wake up at least 1.  A spurious wake up,
     * though, would cause a problem
     */
    dll_append(pp->q, new_jval_i(ps->id)); /* put me on the queue */
    node = dll_last(pp->q);
    while((node != dll_first(pp->q)) || (pp->state[((ps->id+1))%phil_count] == EATING) ||
          (pp->state[(ps->id+(phil_count-1))%phil_count] == EATING)) {
    	pthread_cond_wait(pp->cv[ps->id], pp->mon);
    }
    dll_delete_node(pp->q->flink); /* I must be at the head of the queue */
    pp->state[ps->id] = EATING;
    test_queue(ps->v); /* signal head of the queue if neighbors aren't eating */
  }
  pthread_mutex_unlock(pp->mon);
}
     
/* Set state to thinking and then signal the first person on the
   queue if they can eat */

void putdown(Phil_struct *ps)
{
  Phil *pp;

  pp = (Phil *) ps->v;

  pthread_mutex_lock(pp->mon);
  pp->state[ps->id] = THINKING;
  test_queue(ps->v); /* signal head of the queue if neighbors aren't eating */
  pthread_mutex_unlock(pp->mon);
}

void *initialize_state(int phil_count)
{
  Phil *pp;
  int i;

  pp = (Phil *) malloc(sizeof(Phil)*phil_count);
  pp->phil_count = phil_count;
  pp->mon = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(pp->mon, NULL);
  pp->q = new_dllist();
  for (i = 0; i < phil_count; i++) {
    pp->cv[i] = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
    pthread_cond_init(pp->cv[i], NULL);
    pp->state[i] = THINKING;
  }

  return (void *) pp;
}
