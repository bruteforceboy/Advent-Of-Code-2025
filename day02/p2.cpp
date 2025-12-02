#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
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

auto match(std::string_view s, int len) {
    if (!len || s.length() % len)
        return false;
    auto target = s.substr(0, len);
    for (int i = len; i < s.length(); i += len)
        if (s.substr(i, len) != target)
            return false;
    return true;
}

template <typename T>
auto solve(T vec) -> void {
    long long result = 0;

    for (auto &r : vec) {
        auto splitted = split<std::string>(r, ',');
        for (auto &str : splitted) {
            auto l = stoll(str.substr(0, str.find('-')));
            auto r = stoll(str.substr(str.find('-') + 1));
            for (auto i = l; i <= r; ++i) {
                std::string str = std::to_string(i);

                std::vector lens(str.length(), 0);
                std::iota(begin(lens), end(lens), 0);
                result += std::any_of(std::begin(lens), std::end(lens),
                                      [&](int len) { return match(str, len); })
                              ? i
                              : 0;
            }
        }
    }

    std::cout << result << std::endl;
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
