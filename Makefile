#
DEPENDENCIES= src/main.c src/dl_list.c src/parser.c src/scanner.c src/str.c src/symtable.c
CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -lm -g
INCLUDES=-Isrc/headers

all: main

main: FORCE
	$(CC) $(INCLUDES) $(CFLAGS) -o $@ $(DEPENDENCIES)

clean:
	rm -f main

FORCE:	;