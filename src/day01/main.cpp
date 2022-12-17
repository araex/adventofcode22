#include <filesystem>
#include <fstream>
#include <gsl/gsl>
#include <range/v3/all.hpp>
#include <spdlog/spdlog.h>
#include <string>

namespace fs = std::filesystem;
using namespace ranges;

// Solves https://adventofcode.com/2022/day/1
int main(int /*argc*/, const char ** /*argv*/)
{
    std::ifstream in(fs::path("./data/input.txt"));
    std::string line;

    std::vector<int> elvesCalories;
    int calories = 0;
    while (std::getline(in, line))
    {
        if (line.empty())
        {
            // end of current elfs inventory
            elvesCalories.push_back(calories);
            calories = 0;
            continue;
        }
        calories += std::stoi(line);
    }

    const auto top3 = actions::sort(elvesCalories) | views::reverse | views::take(3);
    spdlog::info("Top elf:     {}", front(top3));
    spdlog::info("Total top 3: {}", accumulate(top3, 0));
}