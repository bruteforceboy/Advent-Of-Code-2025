#include <bits/stdc++.h>
#include <pthread.h>

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

    std::vector<int> xs, ys;
    for (auto &r : vec) {
        int x = stoll(r.substr(0, r.find(',')));
        int y = stoll(r.substr(r.find(',') + 1));
        std::swap(x, y);  // norm??
        xs.emplace_back(x);
        ys.emplace_back(y);
        pts.emplace_back(x, y);
    }

    std::sort(begin(xs), end(xs));
    std::sort(begin(ys), end(ys));
    xs.erase(std::unique(begin(xs), end(xs)), end(xs));
    ys.erase(std::unique(begin(ys), end(ys)), end(ys));

    int mx_x = xs.size(), mx_y = ys.size();
    std::map<int, int> mp_y, mp_x;
    for (int i = 0; i < mx_x; i++)
        mp_x[xs[i]] = i;
    for (int i = 0; i < mx_y; i++)
        mp_y[ys[i]] = i;

    int n = pts.size();
    /*
     * 0 0 0 1 1 1 1
     * 0 0 0 1 0 0 1
     * 1 1 1 1 0 0 1
     * 1 1 1 1 1 1 1
     **/
    std::vector g(mx_x + 1, std::vector(mx_y + 1, 0));

    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        if (pts[i].first == pts[j].first) {
            int y1 = mp_y[pts[i].second], y2 = mp_y[pts[j].second];
            for (int k = std::min(y1, y2); k <= std::max(y1, y2); k++)
                g[mp_x[pts[i].first]][k] = 1;
        } else {
            int x1 = mp_x[pts[i].first], x2 = mp_x[pts[j].first];
            for (int k = std::min(x1, x2); k <= std::max(x1, x2); k++)
                g[k][mp_y[pts[i].second]] = 1;
        }
    }

    for (int i = 0; i <= mx_x; i++) {
        if (count(begin(g[i]), end(g[i]), 1)) {
            int mn = mx_y, mx = 0;
            for (int j = 0; j <= mx_y; j++)
                if (g[i][j])
                    mn = std::min(mn, j), mx = std::max(mx, j);
            for (int j = mn; j <= mx; j++)
                g[i][j] = 1;
        }
    }

    std::vector pref(mx_x + 1, std::vector(mx_y + 1, 0ll));
    for (int i = 0; i <= mx_x; i++) {
        for (int j = 0; j <= mx_y; j++) {
            pref[i][j] = g[i][j];
            if (i)
                pref[i][j] += pref[i - 1][j];
            if (j)
                pref[i][j] += pref[i][j - 1];
            if (i && j)
                pref[i][j] -= pref[i - 1][j - 1];
        }
    }

    auto get_sum = [&](int x1, int y1, int x2, int y2) -> long long {
        long long result = pref[x2][y2];
        if (x1)
            result -= pref[x1 - 1][y2];
        if (y1)
            result -= pref[x2][y1 - 1];
        if (x1 && y1)
            result += pref[x1 - 1][y1 - 1];
        return result;
    };

#ifdef DEBUG
    for (int i = 0; i <= mx_x; i++) {
        for (int j = 0; j <= mx_y; j++)
            std::cout << g[i][j] << ' ';
        std::cout << "\n";
    }
#endif

    long long result = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int x1 = mp_x[pts[i].first], y1 = mp_y[pts[i].second];
            int x2 = mp_x[pts[j].first], y2 = mp_y[pts[j].second];
            if (x1 > x2)
                std::swap(x1, x2);
            if (y1 > y2)
                std::swap(y1, y2);
            if (get_sum(x1, y1, x2, y2) == (x2 - x1 + 1) * (y2 - y1 + 1)) {
                long long dx = abs(pts[i].first - pts[j].first) + 1;
                long long dy = abs(pts[i].second - pts[j].second) + 1;
                result = std::max(result, 1ll * dx * dy);
            }
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
