#include "Cool/spawn_process.hpp"
#include <cassert>
#include <filesystem>
#include <optional>

namespace {
auto string_to_vector_of_char(std::string const& str) -> std::vector<char>
{
    auto res = std::vector<char>(str.begin(), str.end());
    res.push_back('\0');
    return res;
}
} // namespace

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

auto spawn_process_impl(Cool::SpawnProcessArgs const& args) -> std::optional<std::string>
{
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    // Combine the exe path and arguments into a single command line
    std::string command_line = '\"' + args.executable_absolute_path.string() + '\"'; // we need to add quotes in case exe path contains spaces
    for (auto const& arg : args.command_line_args)
        command_line += " \"" + arg + '\"';
    auto cmd = string_to_vector_of_char(command_line); // The API needs a char* so we can't use std::string::c_str()

    if (!CreateProcess(nullptr, cmd.data(), nullptr, nullptr, false, 0, nullptr, args.working_directory.has_value() ? args.working_directory->string().c_str() : nullptr, &si, &pi))
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
auto spawn_process_impl(Cool::SpawnProcessArgs const& args) -> std::optional<std::string>
{
    pid_t pid = fork();

    if (pid < 0)
        return std::string{"Fork failed: "} + strerror(errno);

    if (pid == 0) // We are in the child process:
    {
        if (args.working_directory.has_value())
            chdir(args.working_directory->string().c_str());

        // The API needs an array of char* so we can't use std::string::c_str()
        // So we create all the std::vector<char>, and then in a second std::vector store all their data pointers
        auto args_array_as_vec = std::vector<std::vector<char> >{};
        args_array_as_vec.push_back(string_to_vector_of_char(args.executable_absolute_path.string()));
        for (auto const& arg : args.command_line_args)
            args_array_as_vec.push_back(string_to_vector_of_char(arg));

        auto args_array = std::vector<char*>{};
        for (auto& vec : args_array_as_vec)
            args_array.push_back(vec.data());
        args_array.push_back(nullptr);

        execv(executable_absolute_path.string().c_str(), args_array.data());
    }

    return std::nullopt;
}
} // namespace

#else
#error "Unsupported platform"
#endif

namespace Cool {

auto spawn_process(SpawnProcessArgs const& args) -> std::optional<std::string>
{
    assert(std::filesystem::weakly_canonical(args.executable_absolute_path) == args.executable_absolute_path);
    return spawn_process_impl(args);
}

} // namespace Cool
