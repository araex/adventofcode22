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


    // Flattens a range of ranges by iterating the inner
    // ranges in round-robin fashion.
    template<class Rngs>
    class interleave_view : public ranges::view_facade<interleave_view<Rngs>>
    {
        friend ranges::range_access;
        std::vector<ranges::range_value_t<Rngs>> rngs_;
        struct cursor;
        cursor begin_cursor()
        {
            return {0, &rngs_, ranges::views::transform(rngs_, ranges::begin) | ranges::to<std::vector>};
        }

      public:
        interleave_view() = default;
        explicit interleave_view(Rngs rngs)
          : rngs_(std::move(rngs) | ranges::to<std::vector>)
        {}
    };

    // From https://github.com/ericniebler/range-v3/blob/master/example/calendar.cpp#L192
    // Flattens a range of ranges by iterating the inner
    // ranges in round-robin fashion.
    template<class Rngs>
    struct interleave_view<Rngs>::cursor
    {
        std::size_t n_;
        std::vector<ranges::range_value_t<Rngs>> *rngs_;
        std::vector<ranges::iterator_t<ranges::range_value_t<Rngs>>> its_;
        decltype(auto) read() const
        {
            return *its_[n_];
        }
        void next()
        {
            if(0 == ((++n_) %= its_.size()))
                ranges::for_each(its_, [](auto &it) { ++it; });
        }
        bool equal(ranges::default_sentinel_t) const
        {
            if(n_ != 0)
                return false;
            auto ends = *rngs_ | ranges::views::transform(ranges::end);
            return its_.end() != std::mismatch(its_.begin(), its_.end(), ends.begin(), std::not_equal_to<>{}).first;
        }
        CPP_member auto equal(cursor const &that) const
            -> CPP_ret(bool)(requires ranges::forward_range<ranges::range_value_t<Rngs>>)
        {
            return n_ == that.n_ && its_ == that.its_;
        }
    };

    // From https://github.com/ericniebler/range-v3/blob/master/example/calendar.cpp#L244
    // In:  range<range<T>>
    // Out: range<T>, flattened by walking the ranges
    //                round-robin fashion.
    auto interleave()
    {
        return ranges::make_view_closure([](auto &&rngs) {
            using Rngs = decltype(rngs);
            return interleave_view<ranges::views::all_t<Rngs>>(ranges::views::all(std::forward<Rngs>(rngs)));
        });
    }

    // From https://github.com/ericniebler/range-v3/blob/master/example/calendar.cpp#L256
    // In:  range<range<T>>
    // Out: range<range<T>>, transposing the rows and columns.
    auto transpose()
    {
        return ranges::make_view_closure([](auto &&rngs) {
            using Rngs = decltype(rngs);
            // CPP_assert(ranges::forward_range<Rngs>);
            return std::forward<Rngs>(rngs) | interleave()
                   | ranges::views::chunk(static_cast<std::size_t>(ranges::distance(rngs)));
        });
    }

} // namespace ax