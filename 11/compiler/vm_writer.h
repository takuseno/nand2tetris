#include <string>
#include <stdio.h>
#include <stdlib.h>

// segment
#define CONST 0
#define ARG 1
#define LOCAL 2
#define STATIC 3
#define THIS 4
#define THAT 5
#define POINTER 6
#define TEMP 7

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
    VMWriter();
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
