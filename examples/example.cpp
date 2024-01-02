#include <algorithm>
#include <iostream>
#include <string_view>
#include <vector>
#include <string>
#include <filesystem>

#include "subproc_stream.hpp"

int main(int argc, char **argv) {
    namespace sps = subproc_stream;
    namespace fs = std::filesystem;
    auto args = std::vector<std::string_view>(argv, argv + argc);

    std::cout << "Running command: "
              << "ls -a\n";
    constexpr auto enablelog = true;
    std::string cmd = "ls -a";
    auto p = fs::path("example.log");
    std::cout << "path : " << p << std::endl;
    auto command = sps::SubProc<enablelog>(cmd, "enablelog.log");
    command.exec();
    auto nolog = sps::SubProc<false>(cmd);
    nolog.exec();
    auto c = sps::SubProc<enablelog>(cmd, p); 
    c.exec(); 
    std::cout << "Command Finished: "
              << "ls -a\n";
}
