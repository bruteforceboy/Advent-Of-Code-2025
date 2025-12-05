#include <cassert>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

template <typename T>
auto split(std::string_view str, char delim) {
    std::vector<T> result;
    for (const auto &&word : std::views::split(str, delim)) {
        try {
            auto str = std::string(std::begin(word), std::end(word));
            if constexpr (std::is_integral<T>::value)
                result.push_back(stoll(str));
            else if constexpr (std::is_floating_point<T>::value)
                result.push_back(stold(str));
            else if constexpr (std::is_same_v<T, std::string>)
                result.emplace_back(str);
            else if constexpr (std::is_same_v<T, std::string_view>)
                result.emplace_back(str);
            else
                static_assert(false, "split<T>: wyd blud");
        } catch (...) {
            assert(false && "wyd blud");
        }
    }
    return result;
}

template <typename T>
auto solve(T vec) -> void {
    std::vector<std::pair<long long, long long>> ranges;
    int res = 0;
    for (auto &r : vec) {
        if (r.empty())
            continue;
        if (r.find('-') != std::string::npos) {
            long long lb = stoll(r.substr(0, r.find('-')));
            long long rb = stoll(r.substr(r.find('-') + 1));
            ranges.emplace_back(lb, rb);
        } else {
            long long val = stoll(r);
            bool bad = true;
            for (auto &[l, r] : ranges)
                if (val >= l && val <= r)
                    bad = false;
            res += !bad;
        }
    }
    std::cout << res << std::endl;
}

auto main() -> int {
    std::cin.tie(nullptr)->sync_with_stdio(false);

    constexpr bool SINGLE = true;  // update this!

    std::vector<std::string> lines;
    std::string line;
    while (getline(std::cin, line))
        lines.push_back(std::move(line));

    if constexpr (SINGLE)
        solve(lines);
    else
        for (auto &line : lines)
            solve(line);
}
