all: tokenizer_test compiler

tokenizer_test.o: tokenizer_test.cpp
	g++ -c tokenizer_test.cpp

jack_tokenizer.o: jack_tokenizer.cpp
	g++ -c jack_tokenizer.cpp

compilation_engine.o: compilation_engine.cpp
	g++ -c compilation_engine.cpp

compiler.o: compiler.cpp
	g++ -c compiler.cpp

tokenizer_test: tokenizer_test.o jack_tokenizer.o
	g++ -o tokenizer_test tokenizer_test.o jack_tokenizer.o 

compiler: compiler.o jack_tokenizer.o compilation_engine.o
	g++ -o compiler compiler.o jack_tokenizer.o compilation_engine.o

clean:
	rm *.o
