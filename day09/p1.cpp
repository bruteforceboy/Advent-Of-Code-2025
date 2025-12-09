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
    std::vector<std::pair<int, int>> pts;

    for (auto &r : vec) {
        long long x = stoll(r.substr(0, r.find(',')));
        long long y = stoll(r.substr(r.find(',') + 1));
        pts.emplace_back(x, y);
    }

    int n = pts.size();
    long long res = 0;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            res = std::max(res, 1ll * (abs(pts[j].first - pts[i].first) + 1) *
                                    (abs(pts[j].second - pts[i].second) + 1));
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
