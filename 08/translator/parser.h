#include <string>
#include <fstream>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <vector>

#define C_ARITHMETIC 0
#define C_PUSH 1
#define C_POP 2
#define C_LABEL 3
#define C_GOTO 4
#define C_IF 5
#define C_FUNCTION 6
#define C_RETURN 7
#define C_CALL 8

class Parser {
public:
    Parser(const char* path);
    ~Parser() { ifs_.close(); }
    bool has_more_commands();
    void advance();
    int command_type();
    std::string arg1();
    int arg2();
private:
    std::ifstream ifs_;
    std::string current_line_;
    std::string next_line_;
};
