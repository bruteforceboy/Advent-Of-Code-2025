#include <cassert>
#include <iostream>
#include <ranges>
#include <set>
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
    long long result = 0;
    for (auto &r : vec) {
        auto splitted = split<std::string>(r, ',');
        for (auto &str : splitted) {
            long long l = stoll(str.substr(0, str.find('-')));
            long long r = stoll(str.substr(str.find('-') + 1));
            for (long long i = l; i <= r; i++) {
                std::string i_str = std::to_string(i);
                int n = i_str.length();
                bool can = false;
                for (int i = 1; i < n; i++) {
                    if (n % i == 0) {
                        bool ok = true;
                        std::set<std::string> st;
                        for (int j = 0; j < n; j += i) {
                            st.emplace(i_str.substr(j, i));
                            if (st.size() > 1) {
                                ok = false;
                                break;
                            }
                        }
                        if (ok) {
                            can = true;
                            break;
                        }
                    }
                }
                if (can)
                    result += i;
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
