#pragma once
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace Cool {

/// Returns a non-empty optional containing an error message if something went wrong
/// You can pass command line args (argc / argv) as a vector of strings
/// NB: you don't need to add quotes if your args contain spaces (e.g. `spawn_process(my_exe, {"Hello", "i have many words"})` will send two args: Hello and "i have many words")
auto spawn_process(std::filesystem::path const& executable_absolute_path, std::vector<std::string> const& args = {}) -> std::optional<std::string>;

} // namespace Cool
