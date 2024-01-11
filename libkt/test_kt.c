#include "kos_world.h"
#include "kt.h"

#include <time.h>

#define FORKCOUNT (3)
#define PCOUNT (5000)

void *t1;
void *t2;

kt_sem s1;

void *foo()
{
	fprintf(stdout,"foo called, exiting\n");
	fflush(stdout);
	kt_exit();
	return(NULL);
}

void goo()
{
	fprintf(stdout,"goo calling foo\n");
	fflush(stdout);
	foo();
	fprintf(stdout,"goo back from foo -- problem\n");
	fflush(stdout);
}

void *Thread_1(void *arg)
{
	fprintf(stdout,"I am thread 1 and I'm printing: %s\n",arg);
	fflush(stdout);

	kt_join(t2);

	fprintf(stdout,"I am thread 1 and I've joined t2\n");
	fflush(stdout);

	return(NULL);
}

void *Thread_2(void *arg)
{
	fprintf(stdout,"I am thread 2 and I'm printing: %s\n",arg);
	fflush(stdout);

	fprintf(stdout,"I am thread 2 and I'm calling goo\n");
	fflush(stdout);

	goo();

	kt_exit();

	return(NULL);
}

void *Thread_3(void *arg)
{
	fprintf(stdout,"I am thread 3 and I'm printing: %s\n",arg);
	fflush(stdout);

	kt_exit();


	return(NULL);
}

void *Thread_4(void *arg)
{
	kt_sem s = (kt_sem)arg;

	fprintf(stdout,"I'm thread 4 and I'm synching with thread 5\n");
	fflush(stdout);

	P_kt_sem(s);

	fprintf(stdout,
		"I'm thread 4 and I'm back from synching with thread 5\n");
	fflush(stdout);

	kt_exit();
	return(NULL);
}

void *Thread_5(void *arg)
{
	kt_sem s = (kt_sem)arg;

	fprintf(stdout,"I'm thread 5 and I'm synching with thread 4\n");
	fflush(stdout);

	V_kt_sem(s);

	fprintf(stdout,
		"I'm thread 5 and I'm back from synching with thread 4\n");
	fflush(stdout);

	V_kt_sem(s1);

	kt_exit();
	return(NULL);
}

void *Sleep_T1(void *arg)
{
	int now = time(0);

	fprintf(stdout,"I'm a sleepy task and I'm sleeping for 60 seconds\n");
	fflush(stdout);
	kt_sleep(60);
	if((time(0) - now) < 60)
	{
		fprintf(stdout,
		"I'm a sleepy task and I woke up too quickly after %d secs\n",
	       		time(0) - now);
		fflush(stdout);
	}
	else
	{
		fprintf(stdout,"I'm a sleepy task 3 and I'm awake on time\n");
		fflush(stdout);
	}

	kt_exit();

	return(NULL);
}

void *Sleep_T2(void *arg)
{
	int now = time(0);

	fprintf(stdout,"I'm a sleepy task and I'm sleeping for 5 seconds\n");
	fflush(stdout);
	kt_sleep(5);
	if((time(0) - now) < 5)
	{
		fprintf(stdout,
		"I'm a sleepy task and I woke up too quickly after %d secs\n",
	       		time(0) - now);
		fflush(stdout);
	}
	else
	{
		fprintf(stdout,"I'm a sleepy task 5 and I'm awake on time\n");
		fflush(stdout);
	}

	kt_exit();

	return(NULL);
}

void *Y1(void *arg)
{
	int i;

	fprintf(stdout,"yielder 1 started\n");
	fflush(stdout);
	for(i=0; i < 10000000; i++);

	fprintf(stdout,"yielder 1 yielding\n");
	fflush(stdout);
	kt_yield();

	fprintf(stdout,"yielder 1 ending\n");
	fflush(stdout);

	for(i=0; i < 10000000; i++);

	kt_exit();

	return(NULL);
}

void *Y2(void *arg)
{
	int i;

	fprintf(stdout,"yielder 2 started\n");
	fflush(stdout);
	for(i=0; i < 10000000; i++);

	fprintf(stdout,"yielder 2 yielding\n");
	fflush(stdout);
	kt_yield();

	fprintf(stdout,"yielder 2 ending\n");
	fflush(stdout);

	for(i=0; i < 10000000; i++);

	kt_exit();
	return(NULL);
}

void *Forkee(void *arg)
{
	kt_exit();
	return(NULL);
}

void *Forker(void *arg)
{
	void *t;
	t = kt_fork(Forkee,NULL);
	kt_join(t);

	kt_exit();
	return(NULL);
}

void *Per(void *a)
{
	int i;

	for(i=0; i < PCOUNT; i++)
	{
		P_kt_sem(a);
	}

	kt_exit();
	return(NULL);
}

void *Ver(void *a)
{
	int i;

	for(i=0; i < PCOUNT; i++)
	{
		V_kt_sem(a);
	}

	kt_exit();
	return(NULL);
}



int
main()
{
	char *string1 = "Hi";
	char *string2 = "Mom";
	char *string3 = "How are you?";
	void *t3;
	kt_sem s2;
	struct timeval start;
	struct timeval stop;
	int i;
	double elapsed;

	t1 = kt_fork(Thread_1,string1);
	t2 = kt_fork(Thread_2,string2);
	kt_join(t1);
	fprintf(stdout,"main joined with thread 1\n");
	fflush(stdout);

	t3 = kt_fork(Thread_3,string3);

	fprintf(stdout,"t3 forked\n");
	fflush(stdout);

	kt_joinall();

	fprintf(stdout,"main back from joinall\n");
	fflush(stdout);

	s1 = make_kt_sem(1);
	s2 = make_kt_sem(0);

	t1 = kt_fork(Thread_4,(void *)s2);

	fprintf(stdout,"main forked Thread_4 and calling P on s1\n");
	fflush(stdout);

	P_kt_sem(s1);

	fprintf(stdout,"main out of first P call\n");
	fflush(stdout);

	t2 = kt_fork(Thread_5,(void *)s2);

	fprintf(stdout,"main forked thread 5 and calling P again\n");
	fflush(stdout);

	P_kt_sem(s1);

	fprintf(stdout,"main out of second P call\n");
	fflush(stdout);

	kt_joinall();

	kill_kt_sem(s1);
	kill_kt_sem(s2);

	fprintf(stdout,
		"main back from joinall, timing\n");
	fflush(stdout);

	fprintf(stdout,"%d fork/joins starting...",FORKCOUNT);
	fflush(stdout);
	gettimeofday(&start,NULL);
	for(i=0; i < FORKCOUNT; i++)
	{
		t1 = kt_fork(Forker,NULL);
	}
	kt_joinall();
	gettimeofday(&stop,NULL);
	fprintf(stdout,"done\n");

	elapsed = ((double)((double)stop.tv_sec + 
		   (double)stop.tv_usec/1000000.0) -
	           (double)((double)start.tv_sec +
                   (double)start.tv_usec/1000000.0));
	fprintf(stdout,"%d fork/joins in %3.4fs == %3.4f fj/s\n",
			FORKCOUNT,
			elapsed,
			(double)FORKCOUNT/elapsed);
	fflush(stdout);

	fprintf(stdout,"main back from timing\n");
	fflush(stdout);

	fprintf(stdout,"%d P/V starting...",FORKCOUNT);
	fflush(stdout);

	s1 = make_kt_sem(0);

	gettimeofday(&start,NULL);
	t1 = kt_fork(Per,(void *)s1);
	t2 = kt_fork(Ver,(void *)s1);
	kt_joinall();
	gettimeofday(&stop,NULL);
	fprintf(stdout,"done\n");

	elapsed = ((double)((double)stop.tv_sec + 
		   (double)stop.tv_usec/1000000.0) -
	           (double)((double)start.tv_sec +
                   (double)start.tv_usec/1000000.0));
	fprintf(stdout,"%d P/Vs in %3.4fs == %3.4f pv/s\n",
			PCOUNT,
			elapsed,
			(double)PCOUNT/elapsed);

	fprintf(stdout,"main launching sleepers and yielders\n");
	fflush(stdout);

	kt_fork(Sleep_T1,NULL);
	kt_fork(Sleep_T2,NULL);

	kt_fork(Y1,NULL);
	kt_fork(Y2,NULL);

	t1 = kt_fork(Y1,NULL);
	kt_yield();
	kt_kill(t1);

	fprintf(stdout,"thread Y1 killed\n");
	fflush(stdout);


	kt_joinall();

	fprintf(stdout,"main exiting\n");
	fflush(stdout);



	return(0);
}
	

