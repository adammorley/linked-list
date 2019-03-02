CC=clang
CFLAGS=-I. -g -D_UNIT_TEST=0
DEPS=../log/log.c ../mutex/mutex.c ../node/node.c ../queue/queue.c ../tree-node/tree_node.c ../binary-tree/tree.c

all: test thread-test perf-test

clean:
	rm -f test perf-test *.o

test: test.c linked-list.c $(DEPS)
	$(CC) -o test linked-list.c test.c $(DEPS) $(CFLAGS)

thread-test: thread-test.c linked-list.c $(DEPS)
	$(CC) -o thread-test thread-test.c linked-list.c $(DEPS) $(CFLAGS) -lpthread

perf-test: perf-test.c linked-list.c $(DEPS)
	$(CC) -o perf-test perf-test.c linked-list.c $(DEPS) $(CFLAGS) -lm
