#include "Cool/spawn_process.hpp"
#include <cassert>
#include <filesystem>
#include <optional>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace {
std::string GetLastErrorAsString()
{
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0)
        return "No error";

    LPSTR messageBuffer = nullptr;

    // TODO is this implementation safe ? no strings too long ?
    size_t size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&messageBuffer, 0, NULL
    );

    std::string message(messageBuffer, size);

    LocalFree(messageBuffer);
    return message;
}

auto spawn_process_impl(std::filesystem::path const& executable_absolute_path, std::vector<std::string> const& args) -> std::optional<std::string>
{
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    // Combine the exe path and arguments into a single command line
    std::string command_line = '\"' + executable_absolute_path.string() + '\"'; // we need to add quotes in case exe path contains spaces
    for (auto const& arg : args)
        command_line += " \"" + arg + '\"';
    auto cmd = std::vector<char>(command_line.begin(), command_line.end()); // The API needs a char* so we can't use std::string::c_str()
    cmd.push_back('\0');

    if (!CreateProcess(nullptr, cmd.data(), nullptr, nullptr, false, 0, nullptr, nullptr, &si, &pi))
        return GetLastErrorAsString();

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return std::nullopt;
}
} // namespace

#elif defined(__linux__) || defined(__APPLE__)
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

namespace {
auto spawn_process_impl(std::filesystem::path const& executable_absolute_path, std::vector<std::string> const& args) -> std::optional<std::string>
{
    pid_t pid = fork();

    if (pid < 0)
        return std::string{"Fork failed: "} + strerror(errno);

    if (pid == 0) // We are in the child process:
    {
        auto const exe_path   = executable_absolute_path.string(); // Store the string so that the c_str() is kept alive
        auto       args_array = std::vector<const char*>{};
        args_array.push_back(exe_path.c_str());
        for (auto const& arg : args)
            args_array.push_back(arg.c_str());
        args_array.push_back(nullptr);

        execv(exe_path.c_str(), args_array.data());
    }

    return std::nullopt;
}
} // namespace

#else
#error "Unsupported platform"
#endif

namespace Cool {

auto spawn_process(std::filesystem::path const& executable_absolute_path, std::vector<std::string> const& args) -> std::optional<std::string>
{
    assert(std::filesystem::weakly_canonical(executable_absolute_path) == executable_absolute_path);
    return spawn_process_impl(executable_absolute_path, args);
}

} // namespace Cool
