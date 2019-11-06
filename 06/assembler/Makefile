all: assembler

parser.o: parser.h parser.cpp
	g++ -c parser.cpp

symbol_table.o: symbol_table.h symbol_table.cpp
	g++ -c symbol_table.cpp

code.o: code.h code.cpp
	g++ -c code.cpp

main.o: main.cpp
	g++ -c main.cpp

assembler: parser.o symbol_table.o code.o main.o
	g++ -o assembler parser.o symbol_table.o code.o main.o

clean:
	rm *.o
