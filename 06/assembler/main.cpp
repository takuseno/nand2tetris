#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "code.h"
#include "symbol_table.h"

std::string int_to_bin(int address) {
    std::string bin = "";
    for (int i = 0; i < 15; ++i) {
        if ((address >> (14 - i)) & 0x01)
            bin += "1";
        else
            bin += "0";
    }
    return bin;
}

void fill_symbol_table(const char* path, SymbolTable& table) {
    Parser parser(path);
    int current_address = 0;
    while (parser.has_more_commands()) {
        parser.advance();
        if (parser.command_type() == L_COMMAND)
            table.add_entry(parser.symbol(), current_address);
        else
            ++current_address;
    }
}

void assemble(const char* path, SymbolTable& table) {
    FILE *fp;
    std::string asm_path(path);
    int dot_pos = asm_path.find(".asm");
    std::string bin_path = asm_path.substr(0, dot_pos);
    bin_path += ".hack";
    if ((fp = fopen(bin_path.c_str(), "w")) == NULL) {
        fprintf(stderr, "failed to open %s\n", bin_path.c_str());
        return;
    }

    Parser parser(path);
    Code code;
    int variable_count = 0;
    while (parser.has_more_commands()) {
        parser.advance();
        if (parser.command_type() == C_COMMAND) {
            const char* comp = code.comp(parser.comp()).c_str();
            const char* dest = code.dest(parser.dest()).c_str();
            const char* jump = code.jump(parser.jump()).c_str();
            printf("111%s%s%s\n", comp, dest, jump);
            fprintf(fp, "111%s%s%s\n", comp, dest, jump);
        } else if (parser.command_type() == A_COMMAND) {
            int address;
            std::string symbol = parser.symbol();
            if (!(address = atoi(symbol.c_str())) && symbol[0] != '0') {
                if (!table.contains(symbol)) {
                    table.add_entry(symbol, variable_count + 16);
                    ++variable_count;
                }
                address = table.get_address(symbol);
            }
            std::string bin = int_to_bin(address);
            printf("0%s\n", bin.c_str());
            fprintf(fp, "0%s\n", bin.c_str());
        }
    }

    fclose(fp);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "assembler <path-to-file>\n");
        return 1;
    }

    const char* path = argv[1];
    SymbolTable table;
    fill_symbol_table(path, table);
    assemble(path, table);
    return 0;
}
