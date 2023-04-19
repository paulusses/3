CC = g++ IdentificatorTable.h ConstTable.h WordTable.h IdentificatorTable.cpp WordTable.cpp Test.cpp

all: test clean full_clean

test:
	$(CC)
	./a.out
	make clean

clean:
	rm -f *.gch

full_clean:
	make clean
	rm -f *.out tests

.PHONY: all test clean full_clean

../lab1/
