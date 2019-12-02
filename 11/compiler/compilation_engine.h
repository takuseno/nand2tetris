#include <string>
#include <fstream>
#include <iostream>
#include <exception>
#include <stdexcept>
#include "jack_tokenizer.h"

class CompilationEngine {
public:
    CompilationEngine(JackTokenizer *tokenizer, const char* path);
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
    void compileExpressionList();
private:
    JackTokenizer *tokenizer_;
    FILE *fp_;
    int tmp_token_type_;
    std::string tmp_string_;
    int tmp_int_val_;
    void compileSubroutineCall(std::string identifier);
};
