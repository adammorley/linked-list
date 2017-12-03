CC=/usr/local/Cellar/llvm/5.0.0/bin/clang
CFLAGS=-I. -g

all: list-test thread-test perf-test

clean:
	rm -f list-test perf-test *.o

list-test: list-test.c linked-list.c ../mutex/mutex.c ../node/node.c
	$(CC) -o list-test linked-list.c list-test.c ../mutex/mutex.c ../node/node.c $(CFLAGS)

thread-test: thread-test.c linked-list.c ../mutex/mutex.c ../node/node.c
	$(CC) -o thread-test thread-test.c linked-list.c ../mutex/mutex.c ../node/node.c $(CFLAGS)

perf-test: perf-test.c linked-list.c ../mutex/mutex.c ../node/node.c
	$(CC) -o perf-test perf-test.c linked-list.c ../mutex/mutex.c ../node/node.c $(CFLAGS)
