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
    auto append = true; 
    auto f = sps::SubProcFile("example0.log", append);
    f.write("Hello World\n");
    auto f2 = sps::SubProcFile("example2.log", append);
    f2.write("Hello World\n");
}

void non_blocking_test() {
    auto first = std::make_pair("./test_async.py", "example1.log");
    auto second = std::make_pair("./test_async.py", "example2.log");
    auto third = std::make_pair("./test_async.py", "example3.log"); 
    sps::SubProcNoBlockManager manager(first, second, third);
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
    non_blocking_test();
    test_subproc_file();
}
