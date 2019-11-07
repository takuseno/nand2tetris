#include "parser.h"

Parser::Parser(const char* path) : ifs_(path) {
    advance();
}

bool Parser::has_more_commands() {
    return !next_line_.empty();
}

void Parser::advance() {
    current_line_ = next_line_;
    while (true) {
        std::string tmp;
        if (!getline(ifs_, tmp)) {
            next_line_ = "";
            break;
        }
        // remove spaces
        std::string without_spaces = "";
        for (int i = 0; i < tmp.size(); ++i)
            if (tmp[i] != ' ' && tmp[i] != '\n' && tmp[i] != '\r')
                without_spaces += tmp.substr(i, 1);
        tmp = without_spaces;
        // skip if empty
        if (tmp.empty())
            continue;
        // skip if comments
        if (tmp.substr(0, 2) == "//")
            continue;
        // remove comments
        int pos = tmp.find("//");
        if (pos != std::string::npos) {
            tmp = tmp.substr(0, pos);
        }
        next_line_ = tmp;
        break;
    }
}

int Parser::command_type() {
    if (current_line_.find("push") == 0) return C_PUSH;
    else if (current_line_.find("pop") == 0) return C_POP;
    else if (current_line_.find("label") == 0) return C_LABEL;
    else if (current_line_.find("goto") == 0) return C_GOTO;
    else if (current_line_.find("if-goto") == 0) return C_IF;
    else if (current_line_.find("function") == 0) return C_FUNCTION;
    else if (current_line_.find("call") == 0) return C_CALL;
    else if (current_line_.find("return") == 0) return C_RETURN;
    else return C_ARITHMETIC;
}

std::string extract_until_number(std::string str, int offset) {
    for (int i = offset; i < str.size(); ++i)
        if (str[i] >= '0' && str[i] <= '9')
            return str.substr(offset, i - offset);
    throw std::runtime_error("extract_until_number error");
}

std::string Parser::arg1() {
    if (command_type() == C_RETURN)
        throw std::runtime_error((current_line_ + " arg1 error").c_str());
    switch (command_type()) {
        case C_ARITHMETIC:
            return current_line_;
        case C_LABEL:
            return current_line_.substr(5, current_line_.size() - 5);
        case C_GOTO:
            return current_line_.substr(4, current_line_.size() - 4);
        case C_IF:
            return current_line_.substr(7, current_line_.size() - 7);
    }
    switch (command_type()) {
        case C_PUSH:
            return extract_until_number(current_line_, 4);
        case C_POP:
            return extract_until_number(current_line_, 3);
        case C_FUNCTION:
            return extract_until_number(current_line_, 7);
        case C_CALL:
            return extract_until_number(current_line_, 4);
    }
    throw std::runtime_error("arg1 command_type error");
}

int Parser::arg2() {
    int offset;
    switch (command_type()) {
        case C_PUSH:
            offset = 4;
            break;
        case C_POP:
            offset = 3;
            break;
        case C_FUNCTION:
            offset = 7;
            break;
        case C_CALL:
            offset = 4;
            break;
        default:
            throw std::runtime_error("arg2 command_type error");
    }
    std::string operand = arg1();
    int i = offset + operand.size();
    return atoi(current_line_.substr(i, current_line_.size() - i).c_str());
}
