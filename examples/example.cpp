#include <algorithm>
#include <iostream>
#include <string_view>
#include <vector>
#include <string>
#include <filesystem>
#include <tuple>

#include "subproc_stream.hpp"
namespace sps = subproc_stream;
namespace fs = std::filesystem;

void test_subproc_file() {
    auto f = sps::SubProcFile("example0.log", true);
    f.write("Hello World\n");
    auto f2 = sps::SubProcFile("example2.log", true);
    f2.write("Hello World\n");
}

void non_blocking_test() {
    sps::SubProcNoBlockManager manager(std::make_pair("ls -a", "example1.log"), std::make_pair("ls -a", "example2.log"));
    manager.exec();    
}
int main(int argc, char **argv) {
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
    std::cout << "Running command nonblocking\n";
    test_subproc_file();
    non_blocking_test();
}
