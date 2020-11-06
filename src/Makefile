#
DEPENDENCIES=main.c dl_list.c parser.c scanner.c str.c symtable.c
CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -lm -g

all: main

main: $(DEPENDENCIES)
	$(CC) $(CFLAGS) -o $@ $(DEPENDENCIES)

clean:
	rm -f main