#include <cctype>
#include <charconv>
#include <common/FileReader.h>
#include <common/Views.h>
#include <fmt/ranges.h>
#include <map>
#include <optional>
#include <range/v3/all.hpp>
#include <spdlog/spdlog.h>
#include <stack>
#include <tuple>
#include <unordered_set>

namespace fs = std::filesystem;
namespace rng = ranges;

namespace
{
    int FindMarker(int windowSize)
    {
        const std::vector<std::string> lines = filereader::LineByLine(fs::path("./data/input.txt"));
        int i = 0;
        std::optional<int> firstMarker;
        for(const auto &window : lines[0] | rng::views::sliding(windowSize))
        {
            std::unordered_set<char> uniqueSymbols;
            for(const char c : window)
            {
                const bool wasNew = uniqueSymbols.insert(c).second;
                if(!wasNew)
                {
                    break;
                }
            }
            if(static_cast<int>(uniqueSymbols.size()) == windowSize)
            {
                firstMarker = i + windowSize;
                break;
            }
            ++i;
        }
        return firstMarker.value();
    }
    void Part1()
    {
        spdlog::info("Package marker at {}", FindMarker(4));
    }

    void Part2()
    {
        spdlog::info("Message marker at {}", FindMarker(14));
    }
} // namespace

int main(int /*argc*/, const char ** /*argv*/)
{
    Part1();
    Part2();
}