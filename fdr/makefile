INCL = .
CFLAGS = -g -I$(INCL) 
CC = gcc

KHOME=.

all: $(KHOME)/libfdr.a example-dllist example-jrb my_jval_1 my_jval_2 my_jval_weird my_jval_good

OBJS = fields.o jrb.o jval.o dllist.o


$(KHOME)/libfdr.a: $(OBJS)
	ar ru $(KHOME)/libfdr.a $(OBJS)
	ranlib $(KHOME)/libfdr.a 

clean:
	rm -f core *.o $(KHOME)/libfdr.a

fields.o: fields.c $(INCL)/fields.h $(INCL)/jval.h 
	$(CC) $(CFLAGS) -c fields.c

jval.o: jval.c $(INCL)/jval.h 
	$(CC) $(CFLAGS) -c jval.c

jrb.o: jrb.c $(INCL)/jrb.h  $(INCL)/jval.h
	$(CC) $(CFLAGS) -c jrb.c

dllist.o: dllist.c $(INCL)/dllist.h  $(INCL)/jval.h
	$(CC) $(CFLAGS) -c dllist.c

my_jval_1: $(KHOME)/libfdr.a my_jval_1.c
	$(CC) $(CFLAGS) -o my_jval_1 my_jval_1.c $(KHOME)/libfdr.a

my_jval_2: $(KHOME)/libfdr.a my_jval_2.c
	$(CC) $(CFLAGS) -o my_jval_2 my_jval_2.c $(KHOME)/libfdr.a

my_jval_weird: $(KHOME)/libfdr.a my_jval_weird.c
	$(CC) $(CFLAGS) -o my_jval_weird my_jval_weird.c $(KHOME)/libfdr.a

my_jval_good: $(KHOME)/libfdr.a my_jval_good.c
	$(CC) $(CFLAGS) -o my_jval_good my_jval_good.c $(KHOME)/libfdr.a

example-dllist: $(KHOME)/libfdr.a example-dllist.c
	$(CC) $(CFLAGS) -o example-dllist example-dllist.c $(KHOME)/libfdr.a

example-jrb: $(KHOME)/libfdr.a example-jrb.c
	$(CC) $(CFLAGS) -o example-jrb example-jrb.c $(KHOME)/libfdr.a
