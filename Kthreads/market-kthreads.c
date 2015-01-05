/*
 * cs170 -- Rich wolski
 * producer-consumer example using kthreads
 * uses condition variables for full/empty conditions
 * uses condition variable to fulfill order
 * uses separate lock for each stock
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "c-timer.h"
#include "kt.h"

#define RAND() (drand48())

struct order
{
	int stock_id;
	int quantity;
	int action;	/* buy or sell */
	kt_sem fulfilled;	
};

struct order_que
{
	struct order **orders;
	int size;
	int head;
	int tail;
	kt_sem full;
	kt_sem empty;
};

struct stock
{
	int quantity;
};

struct market
{
	struct stock *stocks;
	int count;
};

struct order *InitOrder(int id, int quantity, int action)
{
	struct order *order;

	order = (struct order *)malloc(sizeof(struct order));
	if(order == NULL) {
		return(NULL);
	}
	order->stock_id = id;
	order->quantity = quantity;
	order->action = action;
	order->fulfilled = make_kt_sem(0);
	return(order);
}

void FreeOrder(struct order *order)
{
	kill_kt_sem(order->fulfilled);
	free(order);
}

struct order_que *InitOrderQue(int size)
{
	struct order_que *oq;

	oq = (struct order_que *)malloc(sizeof(struct order_que));
	if(oq == NULL) {
		return(NULL);
	}
	memset(oq,0,sizeof(struct order_que));

	oq->size = size+1; /* empty condition burns a slot */
	oq->orders = (struct order **)malloc(size*sizeof(struct order *));
	if(oq->orders == NULL) {
		free(oq);
		return(NULL);
	}
	memset(oq->orders,0,size*sizeof(struct order *));

	oq->full = make_kt_sem(size);
	oq->empty = make_kt_sem(0);

	return(oq);
}

void FreeOrderQue(struct order_que *oq)
{
	while(oq->head != oq->tail) {
		FreeOrder(oq->orders[oq->tail]);
		oq->tail = (oq->tail + 1) % oq->size;
	}

	kill_kt_sem(oq->full);
	kill_kt_sem(oq->empty);
	free(oq->orders);
	free(oq);
	return;
}

struct market *InitMarket(int stock_count, int init_quantity)
{
	struct market *m;
	int i;

	m = (struct market *)malloc(sizeof(struct market));
	if(m == NULL) {
		return(NULL);
	}
	m->count = stock_count;

	m->stocks = (struct stock *)malloc(stock_count*sizeof(struct stock));
	if(m->stocks == NULL) {
		free(m);
		return(NULL);
	}

	for(i=0; i < stock_count; i++) {
		m->stocks[i].quantity = init_quantity;
	}

	return(m);
}

void FreeMarket(struct market *m)
{
	free(m->stocks);
	free(m);
	return;
}

void PrintMarket(struct market *m)
{
	int i;
	for(i=0; i < m->count; i++) {
		printf("stock: %d, quantity: %d\n",
			i,m->stocks[i].quantity);
	}

	return;
}

struct client_arg
{
	int id;
	int order_count;
	struct order_que *order_que;
	int max_stock_id;
	int max_quantity;
	int verbose;
};

struct trader_arg
{
	int id;
	struct order_que *order_que;
	struct market *market;
	int *done;
	int verbose;
};

void *ClientThread(void *arg)
{
	struct client_arg *ca = (struct client_arg *)arg;
	int i;
	int next;
	struct order *order;
	int stock_id;
	int quantity;
	int action;
	int queued;
	double now;
	

	for(i=0; i < ca->order_count; i++) {
		/*
		 * create an order for a random stock
		 */
		stock_id = (int)(RAND() * ca->max_stock_id);
		quantity = (int)(RAND() * ca->max_quantity);
		if(RAND() > 0.5) {
			action = 0; /* 0 => buy */
		} else {
			action = 1; /* 1 => sell */
		}
		order = InitOrder(stock_id,quantity,action);
		if(order == NULL) {
			fprintf(stderr,"no space for order\n");
			exit(1);
		}
		/*
		 * queue it for the traders
		 */
		P_kt_sem(ca->order_que->full);
		next = (ca->order_que->head + 1) % ca->order_que->size;
		/*
		 * there is space in the queue, add the order and bump
		 * the head
		 */
		if(ca->verbose == 1) {
			now = CTimer();
			printf("%10.0f client %d: ",now,ca->id);
			printf("queued stock %d, for %d, %s\n",
				order->stock_id,
				order->quantity,
				(order->action ? "SELL" : "BUY")); 
		}
		ca->order_que->orders[next] = order;
		ca->order_que->head = next;
		/*
		 * signal traders that there is another order
		 */
		V_kt_sem(ca->order_que->empty);
		/*
		 * wait until the order is fulfilled
		 */
		P_kt_sem(order->fulfilled);
		/*
		 * done, free the order and repeat
		 */
		FreeOrder(order);
	}

	return(NULL);
}

void *TraderThread(void *arg)
{
	struct trader_arg *ta = (struct trader_arg *)arg;
	int dequeued;
	struct order *order;
	int tail;
	double now;
	int next;
	struct stock *stock;

	while(1) {
		/*
		 * wait until there is a new order
		 */
		P_kt_sem(ta->order_que->empty);
		/*
		 * are we done? Tell the next trader and exit
		 */
		if(*(ta->done) == 1) {
			V_kt_sem(ta->order_que->empty);
			kt_exit();
		}
		/*
		 * get the next order
		 */
		next = (ta->order_que->tail + 1) % ta->order_que->size;
		order = ta->order_que->orders[next];
		ta->order_que->tail = next;
		/*
		 * tell the clients there is another free slot
		 */
		V_kt_sem(ta->order_que->full);
		/*
		 * have an order to process
		 */
		stock = &(ta->market->stocks[order->stock_id]);
		if(order->action == 1) { /* BUY */
			stock->quantity -= order->quantity;
			if(stock->quantity < 0) {
				stock->quantity = 0;
			}
		} else {
			stock->quantity += order->quantity;
		}
		if(ta->verbose == 1) {
			now = CTimer();
			printf("%10.0f trader: %d ",now,ta->id);
			printf("fulfilled stock %d for %d\n",
				order->stock_id,
				order->quantity);
		}
		/*
		 * tell the client the order is done
		 */
		V_kt_sem(order->fulfilled);
	}

	return(NULL);
}

#define ARGS "c:t:o:q:s:V"
char *Usage = "market1 -c clients -t traders -o orders -q queue-size -s stock-count -V <verbose on>\n";

#define INIT_COUNT 5000

int main(int argc, char **argv)
{
	int c;
	int client_threads;
	int trader_threads;
	int orders_per_client;
	int que_size;
	int max_stock;
	int verbose;
	struct client_arg *ca;
	struct trader_arg *ta;
	void **client_ids;
	void **trader_ids;
	struct order_que *order_que;
	struct market *market;
	int i;
	int done;
	int err;
	double start;
	double end;
	

	/*
	 * defaults
	 */
	client_threads = 1;
	trader_threads = 1;
	orders_per_client = 1;
	verbose = 0;
	que_size = 1;
	max_stock = 1;

	while((c = getopt(argc,argv,ARGS)) != EOF) {
		switch(c) {
			case 'c':
				client_threads = atoi(optarg);
				break;
			case 't':
				trader_threads = atoi(optarg);
				break;
			case 'o':
				orders_per_client = atoi(optarg);
				break;
			case 'q':
				que_size = atoi(optarg);
				break;
			case 's':
				max_stock = atoi(optarg);
				break;
			case 'V':
				verbose = 1;
				break;
			default:
				fprintf(stderr,
					"unrecognized command %c\n",
						(char)c);
				fprintf(stderr,"usage: %s",Usage);
				exit(1);
		}
	}

	client_ids = (void **)malloc(client_threads*sizeof(void *));
	if(client_ids == NULL) {
		exit(1);
	}

	ca = (struct client_arg *)malloc(client_threads*sizeof(struct client_arg));
	if(ca == NULL) {
		exit(1);
	}

	trader_ids = (void **)malloc(trader_threads*sizeof(void *));
	if(trader_ids == NULL) {
		exit(1);
	}

	ta = (struct trader_arg *)malloc(trader_threads*sizeof(struct trader_arg));
	if(ta == NULL) {
		exit(1);
	}

	order_que = InitOrderQue(que_size);
	if(order_que == NULL) {
		exit(1);
	}

	market = InitMarket(max_stock,INIT_COUNT);
	if(market == NULL) {
		exit(1);
	}

	start = CTimer();
	for(i=0; i < client_threads; i++) {
		ca[i].id = i;
		ca[i].order_count = orders_per_client;
		ca[i].max_stock_id = max_stock;
		ca[i].max_quantity = INIT_COUNT;
		ca[i].order_que = order_que;
		ca[i].verbose = verbose;
		client_ids[i] = kt_fork(ClientThread,(void *)&ca[i]);
		if(err != 0) {
			fprintf(stderr,"client thread create %d failed\n",i);
			exit(1);
		}
	}

	done = 0;
	for(i=0; i < trader_threads; i++) {
		ta[i].id = i;
		ta[i].order_que = order_que;
		ta[i].market = market;
		ta[i].done = &done;
		ta[i].verbose = verbose;
		trader_ids[i] = kt_fork(TraderThread,(void *)&ta[i]);
		if(err != 0) {
			fprintf(stderr,"trader thread create %d failed\n",i);
			exit(1);
		}
	}

	/*
	 * wait for the clients to finish
	 */
	for(i=0; i < client_threads; i++) {
		kt_join(client_ids[i]);
		if(err != 0) {
			fprintf(stderr,"client join %d failed\n",i);
			exit(1);
		}
	}

	/*
	 * tell the traders we are done
	 */
	done = 1;
	V(order_que->empty);

	for(i=0; i < trader_threads; i++) {
		kt_join(trader_ids[i]);
		if(err != 0) {
			fprintf(stderr,"trader join %d failed\n",i);
			exit(1);
		}
	}
	end = CTimer();

	if(verbose == 1) {
		PrintMarket(market);
	}

	printf("%f transactions / sec\n",
		(double)(orders_per_client*client_threads) / (end-start));

	free(ca);
	free(ta);
	free(client_ids);
	free(trader_ids);
	FreeMarket(market);
	FreeOrderQue(order_que);

	return(0);
}
		


		
		
		

	
	



	
	

