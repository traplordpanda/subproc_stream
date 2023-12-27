#include <algorithm>
#include <iostream>
#include <span>
#include <string_view>
#include <vector>
#include <string>

#include "subproc_stream.hpp"

int main(int argc, char **argv) {
    namespace sps = subproc_stream;
    auto args = std::vector<std::string_view>(argv, argv + argc);

    std::cout << "Running command: "
              << "ls -a\n";
    constexpr auto enablelog = true;
    std::string cmd = "ls -a";
    auto command = sps::SubProc<enablelog>(cmd, "example.log");
    command.exec();
    auto nolog = sps::SubProc<false>(cmd);
    nolog.exec();
    std::cout << "Command Finished: "
              << "ls -a\n";
}
