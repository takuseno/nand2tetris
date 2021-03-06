#include "code_writer.h"
#include <iostream>

CodeWriter::CodeWriter(const char* path) {
    if ((fp_ = fopen(path, "w")) == NULL)
        throw std::runtime_error("failed to open output");
    num_branches_ = 0;
}

void CodeWriter::set_file_name(const char* path) {
    file_name_ = path;
}

void CodeWriter::write_arithmetic(std::string command) {
    if (command == "add" || command == "sub" || command == "and"
            || command == "or") {
        fprintf(fp_, "@SP\n");
        fprintf(fp_, "A=M\n");
        fprintf(fp_, "A=A-1\n");
        fprintf(fp_, "D=M\n"); // retrieve y
        fprintf(fp_, "A=A-1\n");
        if (command == "add")
            fprintf(fp_, "D=D+M\n"); // x = x + y
        else if (command == "sub")
            fprintf(fp_, "D=M-D\n"); // x = x - y
        else if (command == "and")
            fprintf(fp_, "D=M&D\n"); // x = x & y
        else if (command == "or")
            fprintf(fp_, "D=M|D\n"); // x = x | y
        fprintf(fp_, "@SP\n");
        fprintf(fp_, "A=M\n");
        fprintf(fp_, "A=A-1\n");
        fprintf(fp_, "A=A-1\n");
        fprintf(fp_, "M=D\n");
    } else if (command == "neg" || command == "not") {
        fprintf(fp_, "@SP\n");
        fprintf(fp_, "A=M\n");
        fprintf(fp_, "A=A-1\n");
        fprintf(fp_, "D=M\n"); // retrieve y
        if (command == "neg")
            fprintf(fp_, "M=-D\n"); // y = -y
        else if (command == "not")
            fprintf(fp_, "M=!D\n"); // y = !y
    } else if (command == "eq" || command == "gt" || command == "lt") {
        fprintf(fp_, "@SP\n");
        fprintf(fp_, "A=M\n");
        fprintf(fp_, "A=A-1\n");
        fprintf(fp_, "D=M\n"); // retrieve y
        fprintf(fp_, "A=A-1\n");
        fprintf(fp_, "D=M-D\n"); // y = x - y
        fprintf(fp_, "@JUMP%d\n", num_branches_);
        if (command == "eq")
            fprintf(fp_, "D;JEQ\n"); // jump if (y == 0)
        else if (command == "gt")
            fprintf(fp_, "D;JGT\n"); // jump if (y > 0)
        else if (command == "lt")
            fprintf(fp_, "D;JLT\n"); // jump if (y < 0)
        fprintf(fp_, "@0\n");
        fprintf(fp_, "D=A\n");
        fprintf(fp_, "@JUMP%d\n", num_branches_+1);
        fprintf(fp_, "0;JMP\n");
        fprintf(fp_, "(JUMP%d)\n", num_branches_);
        fprintf(fp_, "@0\n");
        fprintf(fp_, "D=!A\n");
        fprintf(fp_, "(JUMP%d)\n", num_branches_+1);
        // store result into stack
        fprintf(fp_, "@SP\n");
        fprintf(fp_, "A=M\n");
        fprintf(fp_, "A=A-1\n");
        fprintf(fp_, "A=A-1\n");
        fprintf(fp_, "M=D\n");
        num_branches_ += 2;
    }
    // move stack pointer
    if (command != "neg" && command != "not") {
        fprintf(fp_, "@SP\n");
        fprintf(fp_, "M=M-1\n");
    }
}

std::string CodeWriter::convert_segment(std::string segment, int index) {
    std::string name = file_name_.substr(0, file_name_.size() - 3);
    if (segment == "constant") return std::to_string(index);
    else if (segment == "argument") return "ARG";
    else if (segment == "local") return "LCL";
    else if (segment == "static") return name;
    else if (segment == "this") return "THIS";
    else if (segment == "that") return "THAT";
    else if (segment == "pointer") return "THIS";
    else if (segment == "temp") return "R5";
    else throw std::runtime_error("convert_segment error");
}

void CodeWriter::write_push_pop(std::string command, std::string segment,
                                int index) {
    std::string symbol = convert_segment(segment, index);
    if (command == "push") {
        // pull data from memory
        if (segment == "static") {
            fprintf(fp_, "@%s.%d\n", symbol.c_str(), index);
            fprintf(fp_, "D=M\n");
        } else if (segment == "constant") {
            fprintf(fp_, "@%d\n", index);
            fprintf(fp_, "D=A\n");
        } else if (segment == "temp") {
            fprintf(fp_, "@R5\n");
            fprintf(fp_, "D=A\n");
            fprintf(fp_, "@%d\n", index);
            fprintf(fp_, "A=D+A\n");
            fprintf(fp_, "D=M\n");
        } else if (segment == "pointer") {
            if (index == 0)
                fprintf(fp_, "@THIS\n");
            else
                fprintf(fp_, "@THAT\n");
            fprintf(fp_, "D=M\n");
        } else {
            fprintf(fp_, "@%s\n", symbol.c_str());
            fprintf(fp_, "D=M\n");
            fprintf(fp_, "@%d\n", index);
            fprintf(fp_, "A=D+A\n");
            fprintf(fp_, "D=M\n");
        }
        // store data into stack
        fprintf(fp_, "@SP\n");
        fprintf(fp_, "A=M\n");
        fprintf(fp_, "M=D\n");
        // increment stack pointer
        fprintf(fp_, "@SP\n");
        fprintf(fp_, "M=M+1\n");
    } else if (command == "pop") {
        // set memory address
        if (segment == "static") {
            fprintf(fp_, "@%s.%d\n", symbol.c_str(), index);
            fprintf(fp_, "D=A\n");
        } else if (segment == "temp") {
            fprintf(fp_, "@R5\n");
            fprintf(fp_, "D=A\n");
            fprintf(fp_, "@%d\n", index);
            fprintf(fp_, "D=D+A\n");
        } else if (segment == "pointer") {
            if (index == 0)
                fprintf(fp_, "@THIS\n");
            else
                fprintf(fp_, "@THAT\n");
            fprintf(fp_, "D=A\n");
        } else {
            fprintf(fp_, "@%s\n", symbol.c_str());
            fprintf(fp_, "D=M\n");
            fprintf(fp_, "@%d\n", index);
            fprintf(fp_, "D=D+A\n");
        }
        fprintf(fp_, "@R13\n"); // use R13 register
        fprintf(fp_, "M=D\n");
        // pull data into D from stack
        fprintf(fp_, "@SP\n");
        fprintf(fp_, "A=M-1\n");
        fprintf(fp_, "D=M\n");
        // store data into memory
        fprintf(fp_, "@R13\n");
        fprintf(fp_, "A=M\n");
        fprintf(fp_, "M=D\n");
        // decrement stack pointer
        fprintf(fp_, "@SP\n");
        fprintf(fp_, "M=M-1\n");
    } else {
        throw std::runtime_error("write_push_pop error");
    }
}

void CodeWriter::close() {
    fclose(fp_);
}
