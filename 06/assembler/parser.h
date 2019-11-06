#include <string>
#include <fstream>
#include <iostream>
#include <exception>
#include <stdexcept>

#define A_COMMAND 0
#define C_COMMAND 1
#define L_COMMAND 2

class Parser {
public:
    Parser(const char* path);
    bool has_more_commands();
    void advance();
    int command_type();
    std::string symbol();
    std::string dest();
    std::string comp();
    std::string jump();

private:
    std::ifstream ifs_;
    std::string current_line_;
    std::string next_line_;
};
