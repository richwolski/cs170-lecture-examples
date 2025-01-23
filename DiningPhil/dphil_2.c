/*
 * CS170: Operating Systems
 * Rich Wolski
 * 	modified from CS460 -- Jim Plank
 * dphil_2.c -- Dining philosophers solution
 * mutex for each stick
 */


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "dphil.h"



typedef struct sticks {
  pthread_mutex_t *lock[MAXTHREADS];
  int phil_count;
} Sticks;


void pickup(Phil_struct *ps)
{
  Sticks *pp;
  int i;
  int phil_count;

  pp = (Sticks *) ps->v;
  phil_count = pp->phil_count;
  
  pthread_mutex_lock(pp->lock[ps->id]);       /* lock up left stick */
  pthread_mutex_lock(pp->lock[(ps->id+1)%phil_count]); /* lock up right stick */ 
}

void putdown(Phil_struct *ps)
{
  Sticks *pp;
  int i;
  int phil_count;

  pp = (Sticks *) ps->v;
  phil_count = pp->phil_count;

  pthread_mutex_unlock(pp->lock[(ps->id+1)%phil_count]); /* unlock right stick */ 
  pthread_mutex_unlock(pp->lock[ps->id]);  /* unlock left stick */
}

void *initialize_state(int phil_count)
{
  Sticks *pp;
  int i;

  pp = (Sticks *) malloc(sizeof(Sticks));

  pp->phil_count = phil_count;
  for (i = 0; i < phil_count; i++) {
    pp->lock[i] = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
  }
  for (i = 0; i < phil_count; i++) {
    pthread_mutex_init(pp->lock[i], NULL);
  }

  return (void *) pp;
}
