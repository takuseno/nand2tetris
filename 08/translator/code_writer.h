#include <string>
#include <vector>
#include <exception>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>

class CodeWriter {
public:
    CodeWriter(const char* path);
    ~CodeWriter() { close(); };
    void set_file_name(const char* path);
    void write_arithmetic(std::string command);
    void write_push_pop(std::string command, std::string segment, int index);
    void write_init();
    void write_label(std::string label);
    void write_goto(std::string label);
    void write_if(std::string label);
    void write_call(std::string function_name, int num_args);
    void write_return();
    void write_function(std::string function_name, int num_locals);
    void close();
private:
    FILE *fp_;
    int num_branches_, num_calls_;
    std::string file_name_;
    std::string current_function_;
    std::vector<std::string> input_files_;
    std::string convert_segment(std::string segment, int index);
    void push_symbol(std::string symbol, bool direct);
};
