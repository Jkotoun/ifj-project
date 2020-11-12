#
DEPENDENCIES= src/main.c src/dl_list.c src/parser.c src/scanner.c src/str.c src/symtable.c src/queue.c
CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -lm -g
INCLUDES=-Isrc/headers
all: main

main: FORCE
	$(CC) $(INCLUDES) $(CFLAGS) -o $@ $(DEPENDENCIES)

test: build 
	cd build && cmake .. && make
	cd build/tests ; ./ScannerTest ; ./ListTest ; ./BTreeTest; ./ParserTest

build:
	mkdir $@ 

clean:
	rm -f main

FORCE:	;