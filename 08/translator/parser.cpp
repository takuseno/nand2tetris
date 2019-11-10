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
        // remove comments
        int pos = tmp.find("//");
        if (pos != std::string::npos) {
            tmp = tmp.substr(0, pos);
        }
        // remove spaces from head and tail
        std::string without_spaces = "";
        bool is_started = false;
        int last_character_pos = -1;
        for (int i = 0; i < tmp.size(); ++i) {
            if (!is_started and tmp[i] != ' ')
                is_started = true;
            if (!is_started)
                continue;
            if (tmp[i] != '\n' && tmp[i] != '\r') {
                without_spaces += tmp.substr(i, 1);
                if (tmp[i] != ' ')
                    last_character_pos = i;
            }
        }
        tmp = without_spaces.substr(0, last_character_pos + 1);
        // skip if empty
        if (tmp.empty())
            continue;
        // skip if comments
        if (tmp.substr(0, 2) == "//")
            continue;
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

std::vector<std::string> break_into_blocks(std::string str) {
    std::vector<std::string> blocks;
    std::string tmp = "";
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] == ' ') {
            if (!tmp.empty())
                blocks.push_back(tmp);
            tmp = "";
            continue;
        }
        tmp += str[i];
    }
    if (!tmp.empty())
        blocks.push_back(tmp);
    return blocks;
}

std::string Parser::arg1() {
    if (command_type() == C_RETURN)
        throw std::runtime_error((current_line_ + " arg1 error").c_str());
    std::vector<std::string> blocks = break_into_blocks(current_line_);
    if (command_type() == C_ARITHMETIC)
        return blocks.at(0);
    return blocks.at(1);
}

int Parser::arg2() {
    if (command_type() != C_PUSH && command_type() != C_POP
            && command_type() != C_FUNCTION && command_type() != C_CALL)
        throw std::runtime_error("arg2 error");
    std::vector<std::string> blocks = break_into_blocks(current_line_);
    return atoi(blocks.at(2).c_str());
}
