#include <cctype>
#include <common/FileReader.h>
#include <filesystem>
#include <fstream>
#include <gsl/gsl>
#include <numeric>
#include <range/v3/all.hpp>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace fs = std::filesystem;
using namespace ranges;

namespace
{
    int getPrio(char c)
    {
        if (std::islower(c))
        {
            return c - 'a' + 1;
        }
        return c - 'A' + 27;
    }


    void Part1()
    {
        std::ifstream in(fs::path("./data/input.txt"));
        std::string line;

        int score = 0;
        while (std::getline(in, line))
        {
            auto compartments =
                line | views::chunk(line.size() / 2) | views::transform([](auto x) { return actions::sort(x); });
            // I'm a bit at a loss here what ranges v3 intends me to do with this now? The following can't be the
            // intended way to do this...
            auto it = begin(compartments);
            auto comp0 = *it++;
            auto comp1 = *it++;
            auto prio = views::set_intersection(comp0, comp1) | views::transform([](char c) { return getPrio(c); });
            score += prio.front();
        }
        spdlog::info("Part1: {}", score);
    }

    void Part2()
    {
        const std::vector<std::string> lines = filereader::LineByLine(fs::path("./data/input.txt"));

        int score = 0;
        for (auto &&group : lines | views::chunk(3))
        {
            auto inventories = group | views::transform([](auto x) { return actions::sort(x); });

            // Can't figure out how to do an accumulate with set_intersection:
            // auto fold = [](const auto &a, const auto &b) { return views::set_intersection(a, b); };
            // auto acc = accumulate(inventories, inventories.front(), fold);
            auto it = begin(inventories);
            auto comp0 = *it++;
            auto comp1 = *it++;
            auto comp2 = *it++;
            auto prio = views::set_intersection(comp0, comp1) | views::set_intersection(comp2)
                        | views::transform([](char c) { return getPrio(c); });
            score += prio.front();
        }
        spdlog::info("Part1: {}", score);
    }
}

int main(int /*argc*/, const char ** /*argv*/)
{
    Part1();
    Part2();
}