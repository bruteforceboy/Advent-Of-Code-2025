#include <cassert>
#include <deque>
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
    std::vector<std::vector<int>> regions;
    std::vector<std::pair<int, int>> regions_dimension;
    std::deque<std::string> rect;
    std::vector<std::vector<std::string>> rectangles;
    for (auto &r : vec) {
        if (r.empty()) {
            rect.pop_front();
            rectangles.emplace_back(begin(rect), end(rect));
            for (auto &r : rectangles.back())
                std::cout << r << std::endl;
            std::cout << std::endl;
            rect.clear();
            continue;
        }
        if (r.find('x') != std::string::npos) {
            regions_dimension.emplace_back(
                stoi(r.substr(0, r.find('x'))),
                stoi(r.substr(r.find('x') + 1, r.find(':') - r.find('x') - 1)));
            std::cout << "region dimension" << std::endl;
            std::cout << regions_dimension.back().first << " "
                      << regions_dimension.back().second << std::endl;
            r = r.substr(r.find(' ') + 1);
            std::cout << "region: " << r << std::endl;
            std::vector<int> region;
            for (auto &v : split<int>(r, ' '))
                region.emplace_back(v);
            regions.emplace_back(region);
        } else {
            rect.push_back(r);
        }
    }

    int n = regions.size(), m = regions[0].size();
    std::vector<int> cnt_rect(m);
    for (int i = 0; i < m; i++)
        for (auto &r : rectangles[i])
            cnt_rect[i] += std::count(begin(r), end(r), '#');

    std::cout << "done" << std::endl;
    std::cout << "n regions: " << n << std::endl;
    int res = 0;
    for (int i = 0; i < n; i++) {
        int n_cells = regions_dimension[i].first * regions_dimension[i].second,
            total = 0;
        for (int j = 0; j < m; j++)
            total += cnt_rect[j] * regions[i][j];
        if (total <= n_cells) {
            res += 1;
            std::cout << "good region: " << i + 1 << std::endl;
        }
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
