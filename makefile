CC=gcc
CFLAGS=-Wall -O3 -pthread

all: double

double: double.c
	$(CC) $(CFLAGS) -o double double.c
