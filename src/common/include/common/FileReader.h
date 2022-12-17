#pragma once

#include <filesystem>
#include <vector>


namespace filereader
{
    std::vector<std::string> LineByLine(const std::filesystem::path &file);
}