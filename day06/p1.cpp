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
            else if constexpr (std::is_same_v<T, std::string>) {
                if (!str.empty())
                    result.emplace_back(str);
            } else if constexpr (std::is_same_v<T, std::string_view>)
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
    std::vector<std::vector<std::string>> rows;

    for (auto &r : vec)
        rows.emplace_back(split<std::string>(r, ' '));

    int n = rows.size(), m = rows[0].size();
    long long result = 0;
    for (int i = 0; i < m; i++) {
        long long cur = (rows[n - 1][i] == "*") ? 1 : 0;
        for (int j = 0; j < n - 1; j++)
            if (rows[n - 1][i] == "+")
                cur += stoll(rows[j][i]);
            else
                cur *= stoll(rows[j][i]);
        result += cur;
    }

    std::cout << result << "\n";
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
