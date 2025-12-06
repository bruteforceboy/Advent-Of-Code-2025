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
    int n = vec.size(), m = 0;
    for (auto &r : vec)
        m = std::max(m, (int) r.size());

    auto parse = [&](int col) {
        long long value = 0;
        for (int i = 0; i < n; i++)
            if (col < vec[i].size() && isdigit(vec[i][col]))
                value = (value * 10) + (vec[i][col] - '0');
        return value;
    };

    long long result = 0;
    for (int col = 0; col < m; col++) {
        auto sym = vec[n - 1][col];
        std::vector<long long> vals;
        for (; parse(col) != 0; col++)
            vals.emplace_back(parse(col));
        if (sym == '+') {
            for (auto &val : vals)
                result += val;
        } else {
            long long cur = 1;
            for (auto &val : vals)
                cur *= val;
            result += cur;
        }
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
