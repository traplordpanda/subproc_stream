#include <array>
#include <fcntl.h>
#include <filesystem>
#include <iostream>
#include <unistd.h>

#include "subproc_stream.hpp"

namespace subproc_stream {
auto SubProc::set_non_blocking(FILE *file) -> void {
    int flags = fcntl(fileno(file), F_GETFL, 0);
    if (flags == -1) {
        throw std::runtime_error("Failed to get file descriptor flags");
    }
    if (fcntl(fileno(file), F_SETFL, flags | O_NONBLOCK) == -1) {
        throw std::runtime_error("Failed to set non-blocking mode");
    }
}
auto SubProc::get_cwd() -> fs::path {
    char cwd[512];
    if (getcwd(cwd, sizeof(cwd)) == nullptr) {
        return fs::path();
    }
    return fs::path(cwd);
}

auto SubProc::close_pipe() -> int { return pclose(pipe_.release()); }
SubProc::SubProc(const std::string_view command)
    : prev_cwd(get_cwd()), pipe_(popen(command.data(), "r"), &pclose) {
    if (!pipe_) {
        throw std::runtime_error("Could not open pipe");
    }
    // set_non_blocking(pipe_.get());
}
// handles if user wants to change cwd as well as run command
SubProc::SubProc(const std::string_view command, const std::string_view cwd)
    : prev_cwd(get_cwd()), chdir_code(chdir(cwd.data())),
      pipe_(popen(command.data(), "r"), &pclose) {
    if (chdir_code != 0) {
        throw std::runtime_error("Could not change directory");
    }
    if (!pipe_) {
        throw std::runtime_error("Could not open pipe");
    }
}
SubProc::~SubProc() {
    if (pipe_) {
        return_code = pclose(pipe_.release());
    }
}
auto SubProc::exec() -> void {
    std::array<char, 128> buffer;
    std::string result;

    while (fgets(buffer.data(), buffer.size(), pipe_.get()) != nullptr) {
        result = buffer.data();
        std::cout << result;
    }
    executed = true;
}
auto SubProc::get_return_code() const -> int{
    if (!executed) {
        throw std::logic_error("Command not executed");
    }
    return return_code;
}
} // namespace subproc_stream 