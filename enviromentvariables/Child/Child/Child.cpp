#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

std::vector<std::string> wrap_command_line(std::size_t argc, char* argv[]) {
    std::vector<std::string> ret;
    for (std::size_t i = 0; i < argc; ++i)
        ret.emplace_back(argv[i]);
    return ret;
}


std::string getArgument(const std::string& name, const std::vector<std::string>& args) {
    auto key = std::find(std::begin(args), std::end(args), name);
    if (key == std::end(args))
        throw std::invalid_argument{ ("command line argument not found: " + name).c_str() };
    if (++key == std::end(args))
        throw std::invalid_argument{ ("command line argument is invalid: " + name).c_str() };
    return *key;
}

std::string getDoubleArg(const std::string& name, const std::vector<std::string>& args) {
    return (getArgument(name, args));
}

int main(int argc, char* argv[])
{
    try {
        std::cout << "child: start" << std::endl;
        auto args = wrap_command_line(argc, argv);
        auto envVariable = getDoubleArg("A", args);
        std::cout <<"A parent enviroment variable is: "<< "\""<< envVariable <<"\"" << std::endl;
        std::cout << "exit_child" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
    }
    return 0;
}