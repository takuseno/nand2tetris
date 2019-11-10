#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include "parser.h"
#include "code_writer.h"

std::string extract_file_name(std::string path) {
    int start_index = 0;
    int end_index = path.size() - 1;
    for (int i = path.size() - 1; i >= 0; --i) {
        if (path[i] == '.')
            end_index = i - 1;
        if (path[i] == '/') {
            start_index = i + 1;
            break;
        }
    }
    return path.substr(start_index, end_index - start_index + 1);
}

void translate(std::string path) {
    std::vector<std::string> paths;
    std::string output_name;
    struct stat st;
    if (stat(path.c_str(), &st) != 0)
        throw std::runtime_error("failed to open");
    if ((st.st_mode & S_IFMT) == S_IFDIR) {
        struct dirent *ent;
        DIR *pDir = opendir(path.c_str());
        std::string base_path = path;
        while ((ent = readdir(pDir)) != NULL) {
            std::string name = ent->d_name;
            if (name.find(".vm") != std::string::npos)
                paths.push_back(base_path + "/" + name);
        }
        closedir(pDir);
    } else {
        paths.push_back(path);
    }

    CodeWriter writer((extract_file_name(path) + ".asm").c_str());

    for (int i = 0; i < paths.size(); ++i) {
        Parser parser(paths[i].c_str());
        writer.set_file_name(extract_file_name(paths[i]).c_str());
        if (i == 0)
            writer.write_init();
        while (parser.has_more_commands()) {
            parser.advance();
            int command_type = parser.command_type();
            if (command_type == C_PUSH)
                writer.write_push_pop("push", parser.arg1(), parser.arg2());
            else if (command_type == C_POP)
                writer.write_push_pop("pop", parser.arg1(), parser.arg2());
            else if (command_type == C_ARITHMETIC)
                writer.write_arithmetic(parser.arg1());
            else if (command_type == C_LABEL)
                writer.write_label(parser.arg1());
            else if (command_type == C_GOTO)
                writer.write_goto(parser.arg1());
            else if (command_type == C_IF)
                writer.write_if(parser.arg1());
            else if (command_type == C_FUNCTION)
                writer.write_function(parser.arg1(), parser.arg2());
            else if (command_type == C_CALL)
                writer.write_call(parser.arg1(), parser.arg2());
            else if (command_type == C_RETURN)
                writer.write_return();
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "translator <path>\n");
        return 1;
    }
    translate(argv[1]);
    return 0;
}
