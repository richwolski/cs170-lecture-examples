#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define INITBALANCE 2000000.0

double BankBalance;

void *ATMThread(void *arg)
{
	int i;
	double local_balance;

	
	for(i=0; i < (int)INITBALANCE/2; i++) {
		local_balance = BankBalance;
		local_balance = local_balance - 1;
		BankBalance = local_balance;
	}

	pthread_exit(NULL);
	return(NULL);
}

int main(int argc, char **argv)
{
	pthread_t t1;
	pthread_t t2;
	int err;

	BankBalance = INITBALANCE;

	printf("initializing bank balance to %f\n",
		BankBalance); 
	fflush(stdout);

	err = pthread_create(&t1,NULL,ATMThread,NULL);
	if(err < 0) {
		fprintf(stderr,"couldn't create first thread\n");
		exit(1);
	}
	err = pthread_create(&t2,NULL,ATMThread,NULL);
	if(err < 0) {
		fprintf(stderr,"couldn't create second thread\n");
		exit(1);
	}

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	printf("final bank balance is %f, should be 0\n",
		BankBalance);
	exit(0);
}
	

	
	


