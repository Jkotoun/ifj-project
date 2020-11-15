
.PHONY: clean

all: main

main: FORCE
	cd src; make

test: build 
	cd build && cmake .. && make
	cd build/tests ; ./ScannerTest ; ./ListTest ; ./BTreeTest; ./ParserTest; ./ExpressionParserTests

expression_test: build 
	cd build && cmake .. && make
	cd build/tests ; ./ExpressionParserTests

build:
	mkdir $@ 

clean:
	cd src; rm -f main

FORCE:	;