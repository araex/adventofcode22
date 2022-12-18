#pragma once

#include <charconv>
#include <range/v3/all.hpp>
#include <string_view>

namespace ax
{
    inline constexpr auto to_string_view
        = ranges::views::transform([](auto &&rng) { return std::string_view(&*rng.begin(), ranges::distance(rng)); });
    inline constexpr auto to_string
        = ranges::views::transform([](auto &&rng) { return std::string(&*rng.begin(), ranges::distance(rng)); });

    inline constexpr auto from_chars = ranges::views::transform([](const std::string_view v) {
        int i;
        std::from_chars(v.data(), v.data() + v.size(), i);
        return i;
    });

    inline constexpr auto stoi = ranges::views::transform([](const std::string &&s) { return std::stoi(s); });
} // namespace ax