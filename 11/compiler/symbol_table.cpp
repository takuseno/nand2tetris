#include "symbol_table.h"

SymbolTable::SymbolTable() {

}

void SymbolTable::startSubroutine() {
    arg_table_.clear();
    var_table_.clear();
}

void SymbolTable::define(std::string name, std::string type, int kind) {
    struct symbol s = {name, type};
    switch (kind) {
        case TB_STATIC:
            static_table_.push_back(s);
            break;
        case TB_FIELD:
            field_table_.push_back(s);
            break;
        case TB_ARG:
            arg_table_.push_back(s);
            break;
        case TB_VAR:
            var_table_.push_back(s);
            break;
        default:
            throw std::runtime_error("SymbolTable define: invalid kind");
    }
}

int SymbolTable::varCount(int kind) {
    switch (kind) {
        case TB_STATIC:
            return static_table_.size();
        case TB_FIELD:
            return field_table_.size();
        case TB_ARG:
            return arg_table_.size();
        case TB_VAR:
            return var_table_.size();
        default:
            throw std::runtime_error("SymbolTable varCount: invalid kind");
    }
}

int SymbolTable::kindOf(std::string name) {
    // search var_table_
    for (int i = 0; i < var_table_.size(); ++i)
        if (var_table_.at(i).name == name)
            return TB_VAR;
    // search arg_table_
    for (int i = 0; i < arg_table_.size(); ++i)
        if (arg_table_.at(i).name == name)
            return TB_ARG;
    // search field_table_
    for (int i = 0; i < field_table_.size(); ++i)
        if (field_table_.at(i).name == name)
            return TB_FIELD;
    // search static_table_
    for (int i = 0; i < static_table_.size(); ++i)
        if (static_table_.at(i).name == name)
            return TB_STATIC;
    return TB_NONE;
}

std::string SymbolTable::typeOf(std::string name) {
    std::vector<struct symbol>* tmp_table = get_table(name);
    for (int i = 0; i < tmp_table->size(); ++i)
        if (tmp_table->at(i).name == name)
            return tmp_table->at(i).type;
    throw std::runtime_error("SymbolTable typeOf: invalid name");
}

int SymbolTable::indexOf(std::string name) {
    std::vector<struct symbol>* tmp_table = get_table(name);
    for (int i = 0; i < tmp_table->size(); ++i)
        if (tmp_table->at(i).name == name)
            return i;
    throw std::runtime_error("SymbolTable indexOf: invalid name");
}

std::vector<struct symbol>* SymbolTable::get_table(std::string name) {
    switch (kindOf(name)) {
        case TB_STATIC:
            return &static_table_;
        case TB_FIELD:
            return &field_table_;
        case TB_ARG:
            return &arg_table_;
        case TB_VAR:
            return &var_table_;
        default:
            throw std::runtime_error("SymbolTable get_table: invalid name");
    }
}
