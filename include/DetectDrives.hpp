#pragma once

#include <filesystem>
#include <optional>
namespace fs = std::filesystem;

std::optional<fs::path> get_new_drive_path();