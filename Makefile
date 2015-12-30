CC=gcc
CFLAGS=-g -std=c99 
LDLIBS=-lpthread
BIN=bin
TEST_BIN=$(BIN)/tests
SRC=src
TEST_SRC=$(SRC)/tests

.PHONY: directories clean clean_tests

all: directories main tests

directories:
	mkdir -p $(BIN) $(TEST_BIN)

tests: test_server test_client

main: $(SRC)/main.c $(SRC)/common.c $(SRC)/client.c $(SRC)/server.c
	$(CC) $(CFLAGS) -o $(BIN)/$@ $^ $(LDLIBS)

test_server: $(TEST_SRC)/test_server.c $(SRC)/server.c $(SRC)/common.c $(SRC)/communication.c
	$(CC) $(CFLAGS) -o $(TEST_BIN)/$@ $^

test_client: $(TEST_SRC)/test_client.c $(SRC)/client.c $(SRC)/common.c $(SRC)/communication.c
	$(CC) $(CFLAGS) -o $(TEST_BIN)/$@ $^

clean:
	rm -rf $(BIN)/main

clean_tests:
	rm -rf $(TEST_BIN)/test_server
