CC = gcc

EXECUTABLES = race1 race2 race3 race_ABC

CSDIR = /cs/faculty/rich/public_html/class/cs170
CFLAGS = -g -I$(CSDIR)/include -Wall

CC = gcc

all: $(EXECUTABLES)

clean:
	rm -f core *.o $(EXECUTABLES) a.out

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $*.c


race1: race1.o
	$(CC) $(CFLAGS) -o race1 race1.o -lpthread

race2: race2.o
	$(CC) $(CFLAGS) -o race2 race2.o -lpthread

race3: race3.o
	$(CC) $(CFLAGS) -o race3 race3.o -lpthread

race_ABC: race_ABC.o
	$(CC) $(CFLAGS) -o race_ABC race_ABC.o -lpthread

