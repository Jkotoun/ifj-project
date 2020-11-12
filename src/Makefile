DEPENDENCIES= main.c dl_list.c parser.c scanner.c str.c symtable.c
CC=gcc
CFLAGS=-std=c11

main: FORCE
	$(CC) $(INCLUDES) $(CFLAGS) -o $@ $(DEPENDENCIES)

FORCE: ;