INCL = .
CFLAGS = -g -I$(INCL) 
CC = gcc

KHOME=.

all: $(KHOME)/libfdr.a example-dllist

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

example-dllist: $(KHOME)/libfdr.a example-dllist.c
	$(CC) $(CFLAGS) -o example-dllist example-dllist.c $(KHOME)/libfdr.a
