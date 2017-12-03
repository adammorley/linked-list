CC=/usr/local/Cellar/llvm/5.0.0/bin/clang
CFLAGS=-I. -g

all: list-test node-test thread-test perf-test

clean:
	rm -f node-test list-test perf-test *.o

node-test: node-test.c node.c
	$(CC) -o node-test node-test.c node.c $(CFLAGS)

list-test: list-test.c linked-list.c node.c ../mutex/mutex.c
	$(CC) -o list-test linked-list.c list-test.c ../mutex/mutex.c node.c $(CFLAGS)

thread-test: thread-test.c node.c linked-list.c ../mutex/mutex.c
	$(CC) -o thread-test thread-test.c linked-list.c ../mutex/mutex.c node.c $(CFLAGS)

perf-test: perf-test.c node.c linked-list.c ../mutex/mutex.c
	$(CC) -o perf-test perf-test.c linked-list.c ../mutex/mutex.c node.c $(CFLAGS)
