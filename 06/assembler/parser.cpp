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
    if (current_line_[0] == '@')
        return A_COMMAND;
    else if (current_line_[0] == '(')
        return L_COMMAND;
    return C_COMMAND;
}

std::string Parser::symbol() {
    if (command_type() == C_COMMAND)
        throw std::runtime_error((current_line_ + " symbol error").c_str());
    if (command_type() == L_COMMAND)
        return current_line_.substr(1, current_line_.size() - 2);
    else
        return current_line_.substr(1, current_line_.size() - 1);
}

std::string Parser::dest() {
    if (command_type() != C_COMMAND)
        throw std::runtime_error((current_line_ + "dest error").c_str());
    int pos = current_line_.find("=");
    if (pos == std::string::npos)
        return "null";
    return current_line_.substr(0, pos);
}

std::string Parser::comp() {
    if (command_type() != C_COMMAND)
        throw std::runtime_error((current_line_ + "comp error").c_str());
    int start_pos = current_line_.find("=");
    if (start_pos == std::string::npos)
        start_pos = 0;
    else
        start_pos += 1;
    int end_pos = current_line_.find(";");
    if (end_pos == std::string::npos)
        end_pos = current_line_.size();
    return current_line_.substr(start_pos, end_pos - start_pos);
}

std::string Parser::jump() {
    if (command_type() != C_COMMAND)
        throw std::runtime_error((current_line_ + "jump error").c_str());
    int pos = current_line_.find(";");
    if (pos == std::string::npos)
        return "null";
    return current_line_.substr(pos + 1, current_line_.size() - pos - 1);
}
