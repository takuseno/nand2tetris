#include <string>
#include <fstream>
#include <iostream>
#include <exception>
#include <stdexcept>
#include "jack_tokenizer.h"
#include "vm_writer.h"
#include "symbol_table.h"

class CompilationEngine {
public:
    CompilationEngine(JackTokenizer *tokenizer, const char* path);
    ~CompilationEngine() {
        delete writer_;
        delete table_;
    }
    void compileClass();
    void compileClassVarDec();
    void compileSubroutine();
    void compileParameterList();
    void compileVarDec();
    void compileStatements();
    void compileDo();
    void compileLet();
    void compileWhile();
    void compileReturn();
    void compileIf();
    void compileExpression();
    void compileTerm();
    int compileExpressionList();
private:
    JackTokenizer *tokenizer_;
    VMWriter *writer_;
    SymbolTable *table_;
    void compileSubroutineCall(std::string identifier);
    void writePushWithVar(std::string identifier, int offset);
    std::string class_name_;
    int while_count_;
    int if_count_;
    int arg_offset_;
};
