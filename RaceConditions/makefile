CC = gcc

EXECUTABLES = race1 race2 race3 race_ABC bank-balance bank-balance-lock unlock-test-1 unlock-test-2

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

unlock-test-1: unlock-test-1.o
	$(CC) $(CFLAGS) -o unlock-test-1 unlock-test-1.o -lpthread

unlock-test-2: unlock-test-2.o
	$(CC) $(CFLAGS) -o unlock-test-2 unlock-test-2.o -lpthread

race_ABC: race_ABC.o
	$(CC) $(CFLAGS) -o race_ABC race_ABC.o -lpthread

bank-balance: bank-balance.o
	$(CC) $(CFLAGS) -o bank-balance bank-balance.o -lpthread

bank-balance-lock: bank-balance-lock.o
	$(CC) $(CFLAGS) -o bank-balance-lock bank-balance-lock.o -lpthread

