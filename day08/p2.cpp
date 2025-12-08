#include <bits/stdc++.h>

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

struct Dsu {
    int n;
    int components;
    std::vector<int> par, sz;
    Dsu(int m) {
        n = m;
        components = m;
        par.resize(n);
        sz.resize(n, 1);
        std::iota(par.begin(), par.end(), 0);
    }

    bool isParent(int u) { return par[u] == u; }

    int getSize(int u) { return sz[u]; }

    int findSet(int u) { return par[u] == u ? u : par[u] = findSet(par[u]); }

    bool unionSet(int u, int v) {
        u = findSet(u);
        v = findSet(v);
        if (u != v) {
            --components;
            if (sz[u] < sz[v])
                std::swap(u, v);
            par[v] = u;
            sz[u] += sz[v];
            return true;
        } else {
            return false;
        }
    }
};

template <typename T>
auto solve(T vec) -> void {
    std::vector<std::vector<int>> pts;
    for (auto &r : vec) {
        auto splitted = split<std::string>(r, ',');
        std::vector<int> vec(3);
        for (int i = 0; i < 3; i++)
            vec[i] = stoi(splitted[i]);
        pts.emplace_back(vec);
    }

    int n = pts.size();
    std::vector<std::tuple<long long, int, int>> conns;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            long long cur_dist = 0;
            for (int x = 0; x < 3; x++)
                cur_dist +=
                    1ll * (pts[i][x] - pts[j][x]) * (pts[i][x] - pts[j][x]);
            conns.emplace_back(cur_dist, i, j);
        }
    }

    std::ranges::sort(conns);

    Dsu dsu(n);
    for (int _ = 0;; _++) {
        auto [dist, x, y] = conns[_];
        dsu.unionSet(x, y);
        if (dsu.getSize(dsu.findSet(x)) == n) {
            std::cout << 1ll * pts[x][0] * pts[y][0] << "\n";
            return;
        }
    }

    std::unreachable();
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
