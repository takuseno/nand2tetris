#include <string>
#include <stdio.h>
#include <stdlib.h>

// segment
#define VM_CONST 0
#define VM_ARG 1
#define VM_LOCAL 2
#define VM_STATIC 3
#define VM_THIS 4
#define VM_THAT 5
#define VM_POINTER 6
#define VM_TEMP 7

// command
#define ADD 0
#define SUB 1
#define NEG 2
#define EQ 3
#define GT 4
#define LT 5
#define AND 6
#define OR 7
#define NOT 8

class VMWriter {
public:
    VMWriter(const char* path);
    void writePush(int segment, int index);
    void writePop(int segment, int index);
    void writeArithmetic(int command);
    void writeLabel(std::string label);
    void writeGoto(std::string label);
    void writeIf(std::string label);
    void writeCall(std::string name, int n_args);
    void writeFunction(std::string name, int n_locals);
    void writeReturn();
    void close();
private:
    FILE* fp_;
};
