#include "Cool/spawn_process.hpp"
#include <cassert>
#include <filesystem>

namespace {

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

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

auto spawn_process_impl(std::filesystem::path const& executable_absolute_path) -> std::optional<std::string>
{
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));
    if (!CreateProcess(executable_absolute_path.string().c_str(), nullptr, nullptr, nullptr, false, 0, nullptr, nullptr, &si, &pi))
        return GetLastErrorAsString();

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return std::nullopt;
}

#elif defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#include <sys/types.h>
#include <cerrno>
#include <cstring>

auto spawn_process_impl(std::filesystem::path const& executable_absolute_path) -> std::optional<std::string>
{
    pid_t pid = fork();

    if (pid < 0)
    {
        // Fork failed
        std::cerr << "Fork failed: " << strerror(errno) << std::endl;
    }
    else if (pid == 0)
    {
        // In child process:
        execl(executable_absolute_path.string().c_str(), executable_absolute_path.string().c_str(), nullptr);
        _exit(1); // Ensure the child process exits
    }
}

#else
#error "Unsupported platform"
#endif
} // namespace

namespace Cool {

auto spawn_process(std::filesystem::path const& executable_absolute_path) -> std::optional<std::string>
{
    assert(std::filesystem::weakly_canonical(executable_absolute_path) == executable_absolute_path);
    return spawn_process_impl(executable_absolute_path);
}

} // namespace Cool
