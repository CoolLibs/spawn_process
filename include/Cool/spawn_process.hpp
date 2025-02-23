#pragma once
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace Cool {

struct SpawnProcessArgs {
    std::filesystem::path                executable_absolute_path{};
    std::vector<std::string>             command_line_args{};
    std::optional<std::filesystem::path> working_directory{};
};

/// Returns a non-empty optional containing an error message if something went wrong
/// You can pass command line args (argc / argv) as a vector of strings
/// NB: you don't need to add quotes if your args contain spaces (e.g. `spawn_process(my_exe, {"Hello", "i have many words"})` will send two args: Hello and "i have many words")
[[nodiscard]] auto spawn_process(SpawnProcessArgs const& args) -> std::optional<std::string>;

} // namespace Cool
