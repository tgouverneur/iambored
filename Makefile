##
# OMG a Makefile for an useless project...
##

CC=gcc

all:	iambored.c
	$(CC) -o iambored iambored.c

dist:	all
	strip -s iambored

clean:
	rm -f ./iambored
