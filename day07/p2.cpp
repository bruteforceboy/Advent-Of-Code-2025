#include <cassert>
#include <iostream>
#include <map>
#include <ranges>
#include <set>
#include <string>
#include <tuple>
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
    int n = vec.size(), m = vec[0].size();
    int splits = 0;

    int x = 0, y = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            if (vec[i][j] == 'S')
                x = i, y = j;
    }

    std::map<std::tuple<int, int, int>, long long> dp;
    const auto solve = [&](auto &&self, int x, int y, int dir) -> long long {
        if (dp.count(std::make_tuple(x, y, dir)))
            return dp[std::make_tuple(x, y, dir)];
        if (x == n - 1)
            return 1;
        long long res = 0;
        if (x + 1 < n && vec[x + 1][y] == '^') {
            res += self(self, x + 1, y - 1, -1);
            res += self(self, x + 1, y + 1, 1);
        } else {
            res += self(self, x + 1, y, 0);
        }
        return dp[std::make_tuple(x, y, dir)] = res;
    };

    std::cout << solve(solve, x, y, 0) << "\n";
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
