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
    void close();
private:
    FILE *fp_;
    int num_branches_;
    std::string file_name_;
    std::vector<std::string> input_files_;
    std::string convert_segment(std::string segment, int index);
};
