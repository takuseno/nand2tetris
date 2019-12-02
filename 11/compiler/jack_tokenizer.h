#include <string>
#include <fstream>
#include <iostream>
#include <exception>
#include <stdexcept>

// token types
#define KEYWORD 0
#define SYMBOL 1
#define IDENTIFIER 2
#define INT_CONST 3
#define STRING_CONST 4

// keyword types
#define CLASS 0
#define METHOD 1
#define FUNCTION 2
#define CONSTRUCTOR 3
#define INT 4
#define BOOLEAN 5
#define CHAR 5
#define VOID 6
#define VAR 7
#define STATIC 8
#define FIELD 9
#define LET 10
#define DO 11
#define IF 12
#define ELSE 13
#define WHILE 14
#define RETURN 15
#define TRUE 16
#define FALSE 17
#define _NULL 18
#define THIS 19

class JackTokenizer {
public:
    JackTokenizer(const char* path);
    bool has_more_tokens();
    void advance();
    int token_type();
    int keyword();
    std::string keyword_as_string();
    std::string symbol();
    std::string identifier();
    int int_val();
    std::string string_val();

private:
    std::ifstream ifs_;
    std::string current_token_;
    std::string next_token_;
    int current_token_type_;
    int next_token_type_;
};
