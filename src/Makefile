DEPENDENCIES= code_gen.c dl_list.c scanner.c str.c symtable.c queue.c expression_stack.c expression.c main.c parser.c
CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -g

main: FORCE
	$(CC) $(INCLUDES) $(CFLAGS) -o $@ $(DEPENDENCIES)

FORCE: ;