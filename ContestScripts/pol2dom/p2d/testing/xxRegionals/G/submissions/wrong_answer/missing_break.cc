#include <bits/stdc++.h>
using namespace std;

const int64_t MOD = 1e9+7;

int64_t f(int at, int fin, vector<int64_t>& dp, vector<vector<int>>& g) {
    if (dp[at] != -1) return dp[at];
    if (at == fin) return dp[at] = 1;
    int64_t res = 0;
    for (int to : g[at]) {
        res += f(to, fin, dp, g);
        res %= MOD;
    }
    return dp[at] = res;
}

int main() {
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
    set<pair<int64_t, int>> Q;
    Q.emplace(0, 0);
    vector<int64_t> dist(N, 1e18);
    dist[0] = 0;
    vector<vector<int>> pars(N);
    while (Q.size()) {
        int64_t d, at;
        tie(d, at) = *Q.begin();
        Q.erase(Q.begin());
        if (d != dist[at]) continue;
        for (auto& e : adj[at]) {
            if (dist[e.first] > d+e.second) {
                dist[e.first] = d+e.second;
                pars[e.first] = {(int)at};
                Q.emplace(dist[e.first], e.first);
            } else if (dist[e.first] == d+e.second) {
                pars[e.first].push_back(at);
            }
        }
    }

    vector<vector<int>> dag(N);
    vector<set<int>> spedges(N);
    for (int i = 0; i < N; ++i)
        for (int j : pars[i]) {
            dag[j].push_back(i);
            spedges[j].insert(i);
        }
    
    vector<int64_t> dpf(N, -1), dpto(N, -1);
    f(0, N-1, dpf, dag);
    f(N-1, 0, dpto, pars);

    for (auto& e : edges) {
        int u, v; tie(u, v) = e;
        if (spedges[u].count(v))
            cout << (dpf[0]-((dpto[u] * dpf[v])%MOD)+MOD)%MOD << endl;
        else
            cout << dpf[0] << endl;
    }
}
