all: tokenizer_test

tokenizer_test.o: tokenizer_test.cpp
	g++ -c tokenizer_test.cpp

jack_tokenizer.o: jack_tokenizer.cpp
	g++ -c jack_tokenizer.cpp

tokenizer_test: tokenizer_test.o jack_tokenizer.o
	g++ -o tokenizer_test tokenizer_test.o jack_tokenizer.o 

clean:
	rm *.o
