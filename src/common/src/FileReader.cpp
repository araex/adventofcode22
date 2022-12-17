#include "FileReader.h"
#include <fstream>

std::vector<std::string> filereader::LineByLine(const std::filesystem::path &file)
{
    std::vector<std::string> lines;
    std::ifstream in(file);
    std::string line;
    while (std::getline(in, line))
    {
        lines.push_back(line);
    }
    return lines;
}