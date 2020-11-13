
.PHONY: clean

all: main

main:
	cd src; make

test: build 
	cd build && cmake .. && make
	cd build/tests ; ./ScannerTest ; ./ListTest ; ./BTreeTest; ./ParserTest

build:
	mkdir $@ 

clean:
	cd src; rm -f main

FORCE:	;