#include <filesystem>
#include <fstream>
#include <gsl/gsl>
#include <optional>
#include <range/v3/all.hpp>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <string>
#include <unordered_map>


namespace fs = std::filesystem;
using namespace ranges;

namespace
{
    enum class Sign
    {
        Rock,
        Paper,
        Scissors
    };

    static const std::unordered_map<char, Sign> parseSign = {
        {'A', Sign::Rock},
        {'B', Sign::Paper},
        {'C', Sign::Scissors},
    };

    enum class Result
    {
        Win,
        Loss,
        Draw
    };

    static const std::unordered_map<char, Result> parseResult = {
        {'X', Result::Loss},
        {'Y', Result::Draw},
        {'Z', Result::Win},
    };

    Sign whatWinsAgainst(Sign sign)
    {
        switch(sign)
        {
        case Sign::Rock:
            return Sign::Paper;
        case Sign::Paper:
            return Sign::Scissors;
        case Sign::Scissors:
            return Sign::Rock;
        }
        std::terminate();
    }

    Sign whatLosesAgainst(Sign sign)
    {
        switch(sign)
        {
        case Sign::Rock:
            return Sign::Scissors;
        case Sign::Paper:
            return Sign::Rock;
        case Sign::Scissors:
            return Sign::Paper;
        }
        std::terminate();
    }

    int getScore(Sign sign)
    {
        switch(sign)
        {
        case Sign::Rock:
            return 1;
        case Sign::Paper:
            return 2;
        case Sign::Scissors:
            return 3;
        }
        std::terminate();
    }

    int getScore(Result result)
    {
        switch(result)
        {
        case Result::Win:
            return 6;
        case Result::Loss:
            return 0;
        case Result::Draw:
            return 3;
        }
        std::terminate();
    }
} // namespace

int main(int /*argc*/, const char ** /*argv*/)
{
    std::ifstream in(fs::path("./data/input.txt"));
    std::string line;

    int totalScore = 0;
    while(std::getline(in, line))
    {
        const Sign theirMove{parseSign.at(line[0])};
        const Result expectedResult{parseResult.at(line[2])};
        totalScore += getScore(expectedResult);

        std::optional<Sign> myMove;
        switch(expectedResult)
        {
        case Result::Win:
            myMove = whatWinsAgainst(theirMove);
            break;
        case Result::Loss:
            myMove = whatLosesAgainst(theirMove);
            break;
        case Result::Draw:
            myMove = theirMove;
            break;
        }

        // Moves also give score
        totalScore += getScore(myMove.value());
    }

    spdlog::info("Total score: {}", totalScore);
}