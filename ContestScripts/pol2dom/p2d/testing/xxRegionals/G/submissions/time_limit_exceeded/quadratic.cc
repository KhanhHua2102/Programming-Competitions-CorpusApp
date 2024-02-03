#include <bits/stdc++.h>
using namespace std;

const int64_t MOD = 1e9+7;

vector<int64_t> dp;
vector<vector<int>> pars;
int64_t f(int at) {
    if (at == 0) return 1;
    if (dp[at] != -1) return dp[at];
    int64_t res = 0;
    for (int to : pars[at]) {
        res += f(to);
        res %= MOD;
    }
    return dp[at] = res;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int N, M;
    cin >> N >> M;
    vector<vector<pair<int, int>>> adj(N);
    vector<pair<int, int>> edges;
    for (int i = 0; i < M; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        --u; --v;
        adj[u].emplace_back(v, w);
        edges.emplace_back(u, v);
    }

    int bannedu = -1, bannedv = -1;
    auto dijk = [&]() {
        priority_queue<pair<int64_t, int>, vector<pair<int64_t, int>>, greater<pair<int64_t, int>>> Q;
        Q.emplace(0, 0);
        vector<int64_t> dist(N, 1e18);
        dist[0] = 0;
        pars.assign(N, {});
        while (Q.size()) {
            int64_t d, at;
            tie(d, at) = Q.top();
            Q.pop();
            if (d != dist[at]) continue;
            for (auto& e : adj[at]) {
                if (at == bannedu && e.first == bannedv) continue;
                if (dist[e.first] > d+e.second) {
                    dist[e.first] = d+e.second;
                    pars[e.first] = {(int)at};
                    Q.emplace(dist[e.first], e.first);
                } else if (dist[e.first] == d+e.second) {
                    pars[e.first].push_back(at);
                }
            }
        }
        if (pars[N-1].empty()) return make_pair((int64_t)0, (int64_t)-1);
        dp.assign(N, -1);
        return make_pair(f(N-1), dist[N-1]);
    };

    int64_t spd = dijk().second;

    for (auto& e : edges) {
        int u, v; tie(u, v) = e;
        bannedu = u; bannedv = v;
        int64_t cnt, dist;
        tie(cnt, dist) = dijk();
        if (dist != spd)
            cout << 0 << "\n";
        else
            cout << cnt << "\n";
    }
    
}
