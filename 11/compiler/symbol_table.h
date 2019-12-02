#include <string>
#include <vector>
#include <exception>
#include <stdexcept>

#define TB_STATIC 0
#define TB_FIELD 1
#define TB_ARG 2
#define TB_VAR 3
#define TB_NONE 4

struct symbol {
    std::string name;
    std::string type;
};

class SymbolTable {
public:
    SymbolTable();
    void startSubroutine();
    void define(std::string name, std::string type, int kind);
    int varCount(int kind);
    int kindOf(std::string name);
    std::string typeOf(std::string name);
    int indexOf(std::string name);
private:
    std::vector<struct symbol> static_table_;
    std::vector<struct symbol> field_table_;
    std::vector<struct symbol> arg_table_;
    std::vector<struct symbol> var_table_;
    std::vector<struct symbol>* get_table(std::string name);
};
