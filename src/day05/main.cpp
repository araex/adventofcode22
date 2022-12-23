#include <cctype>
#include <charconv>
#include <common/FileReader.h>
#include <common/Views.h>
#include <fmt/ranges.h>
#include <map>
#include <range/v3/all.hpp>
#include <spdlog/spdlog.h>
#include <stack>
#include <tuple>

namespace fs = std::filesystem;
namespace rng = ranges;

namespace
{
    bool isNumber(const std::string &s)
    {
        if(s.empty())
        {
            return false;
        }
        const long int numberOfDigits
            = rng::distance(s | rng::views::filter([](const char c) { return std::isdigit(c); }));
        return numberOfDigits == static_cast<long int>(s.size());
    }

    struct Instruction
    {
        int numberOfMoves = -1;
        int from = -1;
        int to = -1;
    };

    Instruction parseInstruction(std::string instruction)
    {
        auto numbers = instruction | rng::views::split(' ') | ax::to_string
                       | rng::views::filter([](const std::string &s) { return isNumber(s); }) | ax::stoi
                       | rng::to_vector;
        assert(numbers.size() == 3);
        return Instruction{numbers[0], numbers[1], numbers[2]};
    }

    void Part1()
    {
        const std::vector<std::string> lines = filereader::LineByLine(fs::path("./data/input.txt"));

        auto stacks = lines
                      | rng::views::split("")
                      // part above the empty line contains the crate
                      | rng::views::take_exactly(1) | rng::views::join
                      | ax::transpose()
                      // We only keep the columns starting with a digit, those are our stacks
                      | rng::views::filter([](auto r) {
                            const char c = *(std::next(r.begin(), rng::distance(r) - 1));
                            return std::isdigit(c);
                        })
                      // remove whitespace
                      | rng::views::transform(
                          [](auto r) { return r | rng::views::filter([](const char c) { return c != ' '; }); })
                      // at this point, the inner range is a forward_range, which makes it impossible to reverse.
                      // we can fix that by evaluating the full range and convert to a vector. This gives us a
                      // range<std::vector<char>>
                      | rng::views::transform([](auto r) { return r | rng::to_vector; });

        std::map<int, std::stack<char>> workspace;
        for(auto rng : stacks)
        {
            const int idx = rng.back() - '0';
            // std::stack will get a iterator constructor with c++23. So for now, do it manually
            workspace[idx] = std::stack<char>();
            for(const char c : rng | rng::views::reverse | rng::views::drop(1))
            {
                workspace[idx].push(c);
            }
        }

        const std::vector<Instruction> instructions
            = lines
              | rng::views::split("")
              // drop the crates, instructions are following
              | rng::views::drop_exactly(1) | rng::views::take_exactly(1) | rng::views::join
              | rng::views::transform([](const std::string &s) { return parseInstruction(s); }) | rng::to_vector;

        for(const auto &instruction : instructions)
        {
            auto &from = workspace.at(instruction.from);
            auto &to = workspace.at(instruction.to);
            for(auto i = 0; i < instruction.numberOfMoves; ++i)
            {
                to.push(from.top());
                from.pop();
            }
        }

        spdlog::info("Part 1");
        for(const auto &pair : workspace)
        {
            spdlog::info("{}: [{}]", pair.first, pair.second.top());
        }
    }

    void Part2()
    {
        const std::vector<std::string> lines = filereader::LineByLine(fs::path("./data/input.txt"));

        auto stacks = lines
                      | rng::views::split("")
                      // part above the empty line contains the crate
                      | rng::views::take_exactly(1) | rng::views::join
                      | ax::transpose()
                      // We only keep the columns starting with a digit, those are our stacks
                      | rng::views::filter([](auto r) {
                            const char c = *(std::next(r.begin(), rng::distance(r) - 1));
                            return std::isdigit(c);
                        })
                      // remove whitespace
                      | rng::views::transform(
                          [](auto r) { return r | rng::views::filter([](const char c) { return c != ' '; }); })
                      // at this point, the inner range is a forward_range, which makes it impossible to reverse.
                      // we can fix that by evaluating the full range and convert to a vector. This gives us a
                      // range<std::vector<char>>
                      | rng::views::transform([](auto r) { return r | rng::to_vector; });

        std::map<int, std::stack<char>> workspace;
        for(auto rng : stacks)
        {
            const int idx = rng.back() - '0';
            // std::stack will get a iterator constructor with c++23. So for now, do it manually
            workspace[idx] = std::stack<char>();
            for(const char c : rng | rng::views::reverse | rng::views::drop(1))
            {
                workspace[idx].push(c);
            }
        }

        const std::vector<Instruction> instructions
            = lines
              | rng::views::split("")
              // drop the crates, instructions are following
              | rng::views::drop_exactly(1) | rng::views::take_exactly(1) | rng::views::join
              | rng::views::transform([](const std::string &s) { return parseInstruction(s); }) | rng::to_vector;

        for(const auto &instruction : instructions)
        {
            auto &from = workspace.at(instruction.from);
            auto &to = workspace.at(instruction.to);

            // Gotta admit i'm a bit tired of fighting ranges::, just do Part2 quick & dirty.
            std::stack<char> temp;
            for(auto i = 0; i < instruction.numberOfMoves; ++i)
            {
                temp.push(from.top());
                from.pop();
            }
            while(!temp.empty())
            {
                to.push(temp.top());
                temp.pop();
            }
        }

        spdlog::info("Part 2");
        for(const auto &pair : workspace)
        {
            spdlog::info("{}: [{}]", pair.first, pair.second.top());
        }
    }
} // namespace

int main(int /*argc*/, const char ** /*argv*/)
{


    // const auto blub = stacks | rng::views::transform([](auto r) { return r | rng::views::reverse |
    // rng::to_vector;
    // });
    // auto print = stacks
    //              // prepare for print with fmt
    //              | rng::views::transform([](auto r) { return r | rng::to_vector; }) | rng::to_vector;
    // spdlog::info("{}", fmt::join(print, "\n"));
    // spdlog::info("{}", print);
    Part1();
    Part2();
}