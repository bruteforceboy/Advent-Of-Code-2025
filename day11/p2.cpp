#include <cassert>
#include <iostream>
#include <map>
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
    std::map<std::string, std::vector<std::string>> g;
    std::vector<std::string> all;

    for (auto &r : vec) {
        std::string label = r.substr(0, r.find(':'));
        r = r.substr(r.find(':') + 2);
        auto to = split<std::string>(r, ' ');
        for (auto &node : to)
            g[label].emplace_back(node);
        all.push_back(label);
    }

    std::map<std::pair<std::string, int>, long long> cur;
    cur[std::make_pair("svr", 0)] = 1;
    long long result = 0;
    while (!cur.empty()) {
        std::map<std::pair<std::string, int>, long long> nvec;
        for (auto &[k, ways] : cur) {
            auto [node, mask] = k;
            for (auto &to : g[node])
                if (to == "dac")
                    nvec[std::make_pair(to, (mask | 1))] += ways;
                else if (to == "fft")
                    nvec[std::make_pair(to, (mask | 2))] += ways;
                else
                    nvec[std::make_pair(to, mask)] += ways;
        }
        for (auto &[k, ways] : nvec)
            if (k.first == "out" && k.second == 3)
                result += ways;
        cur = std::move(nvec);
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
