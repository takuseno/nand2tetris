all: translator

parser.o: parser.h parser.cpp
	g++ -c parser.cpp

code_writer.o: code_writer.h code_writer.cpp
	g++ -c code_writer.cpp

main.o: main.cpp
	g++ -c main.cpp

translator: parser.o code_writer.o main.o
	g++ -o translator parser.o code_writer.o main.o

clean:
	rm *.o
