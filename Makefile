CC=gcc
CFLAGS=-g -std=c99 -Wall -Werror

bin/main: src/main.c
	$(CC) $(CFLAGS) -o $@ $^		
