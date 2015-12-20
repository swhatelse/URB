CC=gcc
CFLAGS=-g -std=c99 -Wall -Werror

all: bin/main bin/server

tests: bin/tests/test_server bin/tests/test_client

bin/main: src/main.c src/common.c src/client.c
	$(CC) $(CFLAGS) -o $@ $^

bin/tests/test_server: src/tests/test_server.c src/server.c
	$(CC) $(CFLAGS) -o $@ $^

bin/tests/test_client: src/tests/test_client.c src/client.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf bin/main

clean_tests:
	rm -rf bin/tests/test_server
