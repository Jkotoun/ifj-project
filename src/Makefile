DEPENDENCIES= main.c dl_list.c parser.c scanner.c str.c symtable.c queue.c expression_stack.c expression.c
CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -g

main: FORCE
	$(CC) $(INCLUDES) $(CFLAGS) -o $@ $(DEPENDENCIES)

FORCE: ;