#pragma once
#include <filesystem>
#include <fcntl.h>
#include <unistd.h>
#include <memory>

namespace subproc_stream
{
    namespace fs = std::filesystem;
    class SubProc
    {
    private:
        fs::path prev_cwd;
        int chdir_code;
        std::unique_ptr<FILE, decltype(&pclose)> pipe_;
        int return_code = -1;
        bool executed = false;

        auto set_non_blocking(FILE *file) -> void;
        auto get_cwd() -> fs::path;
        int close_pipe();

    public:
        SubProc(const std::string_view command);
        SubProc(const std::string_view command, const std::string_view cwd);
        ~SubProc();
        auto exec() -> void;

        auto get_return_code() const -> int;
    };
}