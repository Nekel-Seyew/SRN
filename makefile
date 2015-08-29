cc = gcc
CFLAGS = -O3 -Wall -std=c99
LDFLAGS = -lm -lncurses

TARGET = game

jsmn.o: jsmn/jsmn.c jsmn/jsmn.h
	$(cc) -O3 -c jsmn/jsmn.c

clean:
	rm jsmn.o
