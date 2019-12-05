#include "vm_writer.h"

VMWriter::VMWriter(const char* path) {
    if ((fp_ = fopen(path, "w")) == NULL)
        throw std::runtime_error("VMWriter: failed to open file");
}

void VMWriter::writePush(int segment, int index) {
    switch (segment) {
        case VM_CONST:
            fprintf(fp_, "push constant %d\n", index);
            break;
        case VM_ARG:
            fprintf(fp_, "push argument %d\n", index);
            break;
        case VM_LOCAL:
            fprintf(fp_, "push local %d\n", index);
            break;
        case VM_STATIC:
            fprintf(fp_, "push static %d\n", index);
            break;
        case VM_THIS:
            fprintf(fp_, "push this %d\n", index);
            break;
        case VM_THAT:
            fprintf(fp_, "push that %d\n", index);
            break;
        case VM_POINTER:
            fprintf(fp_, "push pointer %d\n", index);
            break;
        case VM_TEMP:
            fprintf(fp_, "push temp %d\n", index);
            break;
        default:
            throw std::runtime_error("VMWriter writePush: invalid segment");
    }
}

void VMWriter::writePop(int segment, int index) {
    switch (segment) {
        case VM_CONST:
            fprintf(fp_, "pop constant %d\n", index);
            break;
        case VM_ARG:
            fprintf(fp_, "pop argument %d\n", index);
            break;
        case VM_LOCAL:
            fprintf(fp_, "pop local %d\n", index);
            break;
        case VM_STATIC:
            fprintf(fp_, "pop static %d\n", index);
            break;
        case VM_THIS:
            fprintf(fp_, "pop this %d\n", index);
            break;
        case VM_THAT:
            fprintf(fp_, "pop that %d\n", index);
            break;
        case VM_POINTER:
            fprintf(fp_, "pop pointer %d\n", index);
            break;
        case VM_TEMP:
            fprintf(fp_, "pop temp %d\n", index);
            break;
        default:
            throw std::runtime_error("VMWriter writePop: invalid segment");
    }
}

void VMWriter::writeArithmetic(int command) {
    switch (command) {
        case ADD:
            fprintf(fp_, "add\n");
            break;
        case SUB:
            fprintf(fp_, "sub\n");
            break;
        case NEG:
            fprintf(fp_, "neg\n");
            break;
        case EQ:
            fprintf(fp_, "eq\n");
            break;
        case GT:
            fprintf(fp_, "gt\n");
            break;
        case LT:
            fprintf(fp_, "lt\n");
            break;
        case AND:
            fprintf(fp_, "and\n");
            break;
        case OR:
            fprintf(fp_, "or\n");
            break;
        case NOT:
            fprintf(fp_, "not\n");
            break;
        default:
            throw std::runtime_error("VMWriter writeArithmetic: invalid command");
    }
}

void VMWriter::writeLabel(std::string label) {
    fprintf(fp_, "label %s\n", label.c_str());
}

void VMWriter::writeGoto(std::string label) {
    fprintf(fp_, "goto %s\n", label.c_str());
}

void VMWriter::writeIf(std::string label) {
    fprintf(fp_, "if-goto %s\n", label.c_str());
}

void VMWriter::writeCall(std::string name, int n_args) {
    fprintf(fp_, "call %s %d\n", name.c_str(), n_args);
}

void VMWriter::writeFunction(std::string name, int n_locals) {
    fprintf(fp_, "function %s %d\n", name.c_str(), n_locals);
}

void VMWriter::writeReturn() {
    fprintf(fp_, "return\n");
}

void VMWriter::close() {
    fclose(fp_);
}
