#include <charconv>
#include <common/FileReader.h>
#include <common/Views.h>
#include <range/v3/all.hpp>
#include <spdlog/spdlog.h>
#include <tuple>

namespace fs = std::filesystem;
namespace rng = ranges;

namespace
{
    struct Section
    {
        int from = -1;
        int to = -1;
    };
    Section parseSections(const std::string &line)
    {
        auto v = line | rng::views::split('-') | ax::to_string | ax::stoi | rng::to_vector;
        assert(v.size() == 2);
        return Section(v[0], v[1]);
    }

    using Assignment = std::pair<Section, Section>;
    Assignment parseLine(const std::string &line)
    {
        auto v = line | rng::views::split(',') | ax::to_string | rng::views::transform(parseSections) | rng::to_vector;
        assert(v.size() == 2);
        return std::make_pair(v[0], v[1]);
    }

    bool fullyContains(Assignment a)
    {
        auto contains = [](const Section &a, const Section &b) { return a.from <= b.from && a.to >= b.to; };
        return contains(a.first, a.second) || contains(a.second, a.first);
    }

    bool overlaps(Assignment a)
    {
        auto overlaps_ = [](const Section &a, const Section &b) { return a.from <= b.to && a.to >= b.from; };
        return overlaps_(a.first, a.second);
    }

    void Part1()
    {
        const std::vector<std::string> lines = filereader::LineByLine(fs::path("./data/input.txt"));
        spdlog::info("Number of assignments where one section fully contains the other: {}",
                     rng::distance(lines | rng::views::transform(parseLine) | rng::views::filter(fullyContains)));
    }

    void Part2()
    {
        const std::vector<std::string> lines = filereader::LineByLine(fs::path("./data/input.txt"));
        spdlog::info("Number of assignments that overlap: {}",
                     rng::distance(lines | rng::views::transform(parseLine) | rng::views::filter(overlaps)));
    }
} // namespace

int main(int /*argc*/, const char ** /*argv*/)
{
    Part1();
    Part2();
}