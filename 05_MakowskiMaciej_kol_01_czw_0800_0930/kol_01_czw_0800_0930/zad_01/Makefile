CC=gcc
CFLAGS= -std=c17 -Wall -g -O2

bibl1: bibl1.c	
	$(CC) $(CFLAGS) -fPIC -o $@.o -c $<
	$(CC) -shared -o lib$@.so $@.o 

zad1: zad1.c 
	make bibl1 
	$(CC) $(CFLAGS) -o $@.o $< -ldl 

test: zad1.o
	make zad1 
	./$<

clean:
	rm -rf *.so *.o