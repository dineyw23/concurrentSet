CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
PROG = settest
OBJS = settest.o set.o
LIBS = 


$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

settest.o: set.h
set.o: set.h

clean:
	rm -f *~
	rm -f *.o
	rm -rf $(PROG).dSYM
	rm -f $(PROG)