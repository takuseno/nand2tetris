#include <unordered_map>
#include <string>
#include <exception>
#include <stdexcept>

class SymbolTable {
public:
    SymbolTable();
    void add_entry(std::string symbol, int address);
    bool contains(std::string symbol);
    int get_address(std::string symbol);
private:
    std::unordered_map<std::string, int> table_;
};
