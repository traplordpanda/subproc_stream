#pragma once
#include <array>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <string_view>
#include <tuple>
#include <unistd.h>
#include <vector>

namespace fs = std::filesystem;
namespace subproc_stream {
class SubProcFile {
  private:
    std::filesystem::path fpath;
    std::ofstream file;

  public:
    explicit SubProcFile(const fs::path &path, bool append = false)
        : fpath(path),
          file(fpath, append ? std::ios::app : std::ios::binary){};

    ~SubProcFile() { file.close(); };
    // boolean operator to check if file is open
    SubProcFile(SubProcFile &&other) noexcept
        : fpath(std::move(other.fpath)), file(std::move(other.file)) {}
    explicit operator bool() const { return file.is_open(); }
    void write(std::string_view data) { file << data; };
};
template <bool EnableLog> class SubProc {
  private:
    std::unique_ptr<FILE, decltype(&pclose)> pipe_;
    std::conditional_t<EnableLog, SubProcFile, std::nullptr_t> file_log;
    bool executed = false;
    int fd;
    int close_pipe() { return pclose(pipe_.release()); }

  public:
    explicit SubProc(std::string_view command)
        : pipe_(popen(command.data(), "r"), &pclose), fd(fileno(pipe_.get())) {
        if (!pipe_) {
            throw std::runtime_error("Could not open pipe");
        }
        if (fd < 0) {
            throw std::runtime_error("Could not get file descriptor");
        }
    }

    template <typename T>
    SubProc(std::string_view command, const T &file_log_path)
        : pipe_(popen(command.data(), "r"), &pclose), file_log(file_log_path),
          fd(fileno(pipe_.get())) {

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

template <bool EnableLog> class SubProcNoBlock {
  private:
    std::unique_ptr<FILE, decltype(&pclose)> pipe_;
    std::conditional_t<EnableLog, SubProcFile, std::nullptr_t> file_log;
    bool executed = false;
    int fd;
    int close_pipe() { return pclose(pipe_.release()); }
    auto set_non_blocking() const -> void {
        int flags = fcntl(fd, F_GETFL, O_NONBLOCK);
        if (flags == -1) {
            throw std::runtime_error("Failed to get file descriptor flags");
        }
        if (fcntl(fileno(pipe_.get()), F_SETFL, flags | O_NONBLOCK) == -1) {
            throw std::runtime_error("Failed to set non-blocking mode");
        }
    }

  public:
    static enum class State { RUNNING, DONE, ERROR } state;
    explicit SubProcNoBlock(std::string_view command)
        : pipe_(popen(command.data(), "r"), &pclose), fd(fileno(pipe_.get())) {
        if (!pipe_) {
            throw std::runtime_error("Could not open pipe");
        }
        if (fd < 0) {
            throw std::runtime_error("Could not get file descriptor");
        }
        set_non_blocking();
    }

    template <typename T>
    SubProcNoBlock(std::string_view command, const T &file_log_path)
        : pipe_(popen(command.data(), "r"), &pclose),
          file_log(file_log_path, true), fd(fileno(pipe_.get())) {

        if (!pipe_) {
            throw std::runtime_error("Could not open pipe");
        }
        set_non_blocking();
    }

    SubProcNoBlock(SubProcNoBlock &&other) noexcept
        : pipe_(std::move(other.pipe_)), file_log(std::move(other.file_log)), fd(std::move(other.fd)) {}
    // Move assignment operator
    SubProcNoBlock &operator=(SubProcNoBlock &&other) noexcept {
        if (this != &other) {
            pipe_ = std::move(other.pipe_);
        }
        return *this;
    }

    // Delete the copy constructor and copy assignment operator
    SubProcNoBlock(const SubProcNoBlock &) = delete;
    SubProcNoBlock &operator=(const SubProcNoBlock &) = delete;

    auto exec() -> State {
        std::array<char, 256> buffer;
        ssize_t r = 0;
        r = read(fd, buffer.data(), buffer.size());
        if (r == -1 && errno == EAGAIN) {
            // No data available right now
            return State::RUNNING;
        } else if (r > 0) {
            std::string result(buffer.data(), static_cast<std::size_t>(r));
            if constexpr (EnableLog) {
                file_log.write(result);
            }
            std::cout << result;
            return State::RUNNING;
        } else {
            executed = true;
            return State::DONE;
        }
    }
};

class SubProcNoBlockManager {
  private:
    std::vector<SubProcNoBlock<true>> subprocs;
    auto add_proc(std::pair<const char *, const char *> command_path) -> void {
        subprocs.emplace_back(std::get<0>(command_path),
                              std::get<1>(command_path));
    }

  public:
    SubProcNoBlockManager() = default;
    template <typename... Args,
              typename = std::enable_if_t<!std::disjunction_v<
                  std::is_same<std::decay_t<Args>, SubProcNoBlockManager>...>>>
    explicit SubProcNoBlockManager(Args... args) {
        (add_proc(args), ...);
    }
    auto exec() -> void {
        while (!subprocs.empty()) {
            for (std::size_t i = 0; i < subprocs.size(); ++i) {
                auto &proc = subprocs[i];
                if (proc.exec() == SubProcNoBlock<true>::State::DONE) {
                    auto suprocsi = subprocs.begin() + static_cast<long>(i);
                    subprocs.erase(suprocsi);
                }
            }
        }
    }
};
} // namespace subproc_stream
