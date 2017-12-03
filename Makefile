CC=/usr/local/Cellar/llvm/5.0.0/bin/clang
CFLAGS=-I. -g

all: test thread-test perf-test

clean:
	rm -f test perf-test *.o

test: test.c linked-list.c ../mutex/mutex.c ../node/node.c
	$(CC) -o test linked-list.c test.c ../mutex/mutex.c ../node/node.c $(CFLAGS)

thread-test: thread-test.c linked-list.c ../mutex/mutex.c ../node/node.c
	$(CC) -o thread-test thread-test.c linked-list.c ../mutex/mutex.c ../node/node.c $(CFLAGS)

perf-test: perf-test.c linked-list.c ../mutex/mutex.c ../node/node.c
	$(CC) -o perf-test perf-test.c linked-list.c ../mutex/mutex.c ../node/node.c $(CFLAGS)
