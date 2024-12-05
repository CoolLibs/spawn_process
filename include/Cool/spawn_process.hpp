#pragma once
#include <filesystem>
#include <optional>
#include <string>

namespace Cool {

/// Returns a non-empty optional containing an error message if something went wrong
auto spawn_process(std::filesystem::path const& executable_absolute_path) -> std::optional<std::string>;

} // namespace Cool
