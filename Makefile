CC=/usr/local/Cellar/gcc/7.2.0/bin/gcc-7
CFLAGS=-I. -g

list-test: list-test.c linked-list.c node.c
	$(CC) -o list-test linked-list.c list-test.c node.c $(CFLAGS)

node-test: node-test.c node.c
	$(CC) -o node-test node-test.c node.c $(CFLAGS)
