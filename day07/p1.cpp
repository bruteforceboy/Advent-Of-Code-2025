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
    int n = vec.size(), m = vec[0].size();
    int splits = 0;
    while (true) {
        auto nvec = vec;
        bool passed = false;
        for (int i = 1; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (vec[i - 1][j] == 'S') {
                    passed = true;
                    if (vec[i][j] == '^') {
                        vec[i + 1][j - 1] = 'S';
                        vec[i + 1][j + 1] = 'S';
                        ++splits;
                    } else {
                        vec[i][j] = 'S';
                    }
                    vec[i - 1][j] = '.';
                }
            }
        }
        if (!passed)
            break;
    }
    std::cout << splits << std::endl;
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
