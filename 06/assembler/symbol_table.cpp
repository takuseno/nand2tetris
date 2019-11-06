#include "symbol_table.h"

SymbolTable::SymbolTable() {
    add_entry("SP", 0);
    add_entry("LCL", 1);
    add_entry("ARG", 2);
    add_entry("THIS", 3);
    add_entry("THAT", 4);
    add_entry("R0", 0);
    add_entry("R1", 1);
    add_entry("R2", 2);
    add_entry("R3", 3);
    add_entry("R4", 4);
    add_entry("R5", 5);
    add_entry("R6", 6);
    add_entry("R7", 7);
    add_entry("R8", 8);
    add_entry("R9", 9);
    add_entry("R10", 10);
    add_entry("R11", 11);
    add_entry("R12", 12);
    add_entry("R13", 13);
    add_entry("R14", 14);
    add_entry("R15", 15);
    add_entry("SCREEN", 16384);
    add_entry("KBD", 24576);
}

void SymbolTable::add_entry(std::string symbol, int address) {
    table_[symbol] = address;
}

bool SymbolTable::contains(std::string symbol) {
    return !(table_.find(symbol) == table_.end());
}

int SymbolTable::get_address(std::string symbol) {
    if (!contains(symbol))
        throw std::runtime_error("get_address error");
    return table_[symbol];
}
