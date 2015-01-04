#ifndef SEMA_H
#define SEMA_H

#include <pthread.h>

struct sem
{
        pthread_mutex_t lock;
        pthread_cond_t wait;
        int count;
};

typedef struct sem sema;

sema *InitSem(int init_val);
void FreeSem(sema *s);
void P(sema *s);
void V(sema *s);

#endif

