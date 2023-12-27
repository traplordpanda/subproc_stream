#pragma once
#include <array>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string_view>
#include <unistd.h>

namespace fs = std::filesystem;
namespace subproc_stream {
class SubProcFile {
  private:
    std::filesystem::path fpath;
    std::ofstream file;

  public:
    explicit SubProcFile(std::string_view path)
        : fpath(path.data()), file(fpath, std::ios::binary){};
    ~SubProcFile() { file.close(); };
    // boolean operator to check if file is open
    explicit operator bool() const { return file.is_open(); }
    void write(std::string_view data) { file << data; };
};
template <bool EnableLog>
class SubProc {
  private:
    std::unique_ptr<FILE, decltype(&pclose)> pipe_;
    std::conditional_t<EnableLog, SubProcFile, std::nullptr_t> file_log;
    bool executed = false;

    auto set_non_blocking() const -> void {
        int flags = fcntl(fileno(pipe_.get()), F_GETFL, 0);
        if (flags == -1) {
            throw std::runtime_error("Failed to get file descriptor flags");
        }
        if (fcntl(fileno(pipe_.get()), F_SETFL, flags | O_NONBLOCK) == -1) {
            throw std::runtime_error("Failed to set non-blocking mode");
        }
    }

    auto get_cwd() const -> fs::path {
        constexpr auto max_path_size = 512;
        std::array<char, max_path_size> cwd;
        if (getcwd(cwd.data(), cwd.size()) == nullptr) {
            return fs::path();
        }
        return fs::path(cwd.data());
    }

    int close_pipe() { return pclose(pipe_.release()); }

  public:
    explicit SubProc(std::string_view command)
        : pipe_(popen(command.data(), "r"), &pclose) {
        if (!pipe_) {
            throw std::runtime_error("Could not open pipe");
        }
    }
    SubProc(std::string_view command, std::string_view file_log_path)
        : pipe_(popen(command.data(), "r"), &pclose), file_log(file_log_path) {

        if (!pipe_) {
            throw std::runtime_error("Could not open pipe");
        }
    }
    
    auto exec() -> void {
        std::array<char, 128> buffer;
        std::string result;

        while (fgets(buffer.data(), buffer.size(), pipe_.get()) != nullptr) {
            result = buffer.data();
            if constexpr (EnableLog) {
                file_log.write(buffer.data());
            }
            std::cout << result;
        }
        executed = true;
    }
};
} // namespace subproc_stream