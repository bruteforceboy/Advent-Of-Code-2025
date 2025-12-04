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
    int n = vec.size(), m = vec[0].size(), res = 0;

    while (true) {
        std::vector changed(n, std::vector(m, false));
        int removed = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (vec[i][j] != '@')
                    continue;
                const int dx[] = {-1, 0, 1, 0, -1, 1, 1, -1};
                const int dy[] = {0, 1, 0, -1, -1, 1, -1, 1};

                auto in = [&](int x, int y) {
                    return (x >= 0 && x < n && y >= 0 && y < m);
                };

                int tot = 0;
                for (int dir = 0; dir < 8; dir++) {
                    int x = i + dx[dir];
                    int y = j + dy[dir];
                    if (in(x, y))
                        tot += vec[x][y] == '@';
                }

                if (tot < 4) {
                    ++removed;
                    changed[i][j] = true;
                }
            }
        }

        if (!removed)
            break;

        res += removed;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++)
                if (changed[i][j])
                    vec[i][j] = '.';
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
