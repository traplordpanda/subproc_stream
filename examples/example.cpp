#include <algorithm>
#include <iostream>
#include <string_view>
#include <span>
#include <vector>

#include "subproc_stream.hpp"

int main(int argc, char** argv){
    namespace sps = subproc_stream;
    auto args = std::vector<std::string_view>(argv, argv + argc); 
    
    std::cout << "Running command: " << "ls\n";
    auto command = sps::SubProc("ls -a");
    command.exec();
    std::cout << "Command Finished: " << "ls\n";
}
