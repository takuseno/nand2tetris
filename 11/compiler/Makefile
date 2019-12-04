all: compiler

jack_tokenizer.o: jack_tokenizer.cpp jack_tokenizer.h
	g++ -c jack_tokenizer.cpp

compilation_engine.o: compilation_engine.cpp compilation_engine.h
	g++ -c compilation_engine.cpp

symbol_table.o: symbol_table.cpp symbol_table.h
	g++ -c symbol_table.cpp

vm_writer.o: vm_writer.cpp vm_writer.h
	g++ -c vm_writer.cpp

compiler.o: compiler.cpp
	g++ -c compiler.cpp

compiler: compiler.o jack_tokenizer.o compilation_engine.o symbol_table.o vm_writer.o
	g++ -o compiler compiler.o jack_tokenizer.o compilation_engine.o symbol_table.o vm_writer.o

clean:
	rm *.o
