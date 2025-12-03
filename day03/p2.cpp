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

template <typename T>
auto solve(T vec) -> void {
    long long res = 0;
    for (auto &r : vec) {
        int len = r.size();

        std::vector<long long> p(13);
        p[0] = 1;
        for (int i = 1; i <= 12; i++)
            p[i] = p[i - 1] * 10;

        std::vector dp(len + 1, std::vector<long long>(13, -1));
        dp[len][0] = 0;
        for (int i = len - 1; i >= 0; i--) {
            for (int j = 0; j <= 11; j++)
                if (dp[i + 1][j] != -1)
                    dp[i][j + 1] = std::max(
                        dp[i][j + 1], (r[i] - '0') * p[j + 1] + dp[i + 1][j]);
            for (int j = 0; j <= 12; j++)
                dp[i][j] = std::max(dp[i][j], dp[i + 1][j]);
        }

        res += dp[0][12] / 10;
    }
    std::cout << res << "\n";
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
