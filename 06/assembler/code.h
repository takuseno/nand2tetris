#include <string>
#include <exception>
#include <stdexcept>

class Code {
public:
    Code() {};
    std::string dest(std::string str);
    std::string comp(std::string str);
    std::string jump(std::string str);
};
