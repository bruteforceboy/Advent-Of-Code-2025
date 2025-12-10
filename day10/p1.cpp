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
    int total = 0;
    for (auto &r : vec) {
        auto splitted = split<std::string>(r, ' ');
        std::string mask;
        std::vector<std::vector<int>> pushes;
        for (auto &str : splitted) {
            if (str[0] == '[') {
                mask = str.substr(1, str.length() - 2);
            } else if (str[0] == '(' || str[0] == '{') {
                auto indices = str.substr(1, str.length() - 2);
                std::vector<int> idx;
                for (auto &id : split<std::string>(indices, ','))
                    idx.emplace_back(stoi(id));
                pushes.emplace_back(idx);
            } else {
                assert(false);
            }
        }
        pushes.pop_back();
        int n = pushes.size(), res = n;
        for (int i = 0; i < (1 << n); i++) {
            std::string tmp = mask;
            for (int j = 0; j < n; j++) {
                if ((i >> j) & 1)
                    for (auto &button : pushes[j])
                        tmp[button] = (tmp[button] == '.' ? '#' : '.');
            }
            if (count(begin(tmp), end(tmp), '.') == mask.size())
                res = std::min(res, __builtin_popcount(i));
        }
        std::cout << "res: " << res << std::endl;
        total += res;
    }
    std::cout << "total: " << total << std::endl;
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
