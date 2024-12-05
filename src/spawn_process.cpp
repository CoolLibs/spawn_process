#include "Cool/spawn_process.hpp"
#include <cassert>
#include <filesystem>

namespace {

#if defined(_WIN32)
#include <windows.h>

std::string GetLastErrorAsString()
{
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0)
    {
        return "No error";
    }

    LPSTR messageBuffer = nullptr;

    size_t size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&messageBuffer, 0, NULL
    );

    std::string message(messageBuffer, size);

    // Free the buffer allocated by FormatMessage
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
