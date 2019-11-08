#include "parser.h"
#include "code_writer.h"
#include <string>


void translate(std::string path) {
    std::string name = path;
    if (name.find(".vm") != std::string::npos)
        name = name.substr(0, name.size() - 3);

    CodeWriter writer((name + ".asm").c_str());

    // TODO: deal with multiple files
    Parser parser(path.c_str());
    writer.set_file_name(path.c_str());

    while (parser.has_more_commands()) {
        parser.advance();
        int command_type = parser.command_type();
        if (command_type == C_PUSH)
            writer.write_push_pop("push", parser.arg1(), parser.arg2());
        else if (command_type == C_POP)
            writer.write_push_pop("pop", parser.arg1(), parser.arg2());
        else if (command_type == C_ARITHMETIC)
            writer.write_arithmetic(parser.arg1());
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
