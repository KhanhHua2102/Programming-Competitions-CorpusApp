#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int N, M, T;
    cin >> N >> M;
    T = N-1;
    vector<int> is_source(N, 1), outgoing(M);
    vector<vector<int>> in_edge(N), incoming(M);
    int k, to, from;
    for (int i = 0; i < M; ++i) {
        cin >> to >> k;
        --to;
        outgoing[i] = to;
        is_source[to] = 0;
        for (int j = 0; j < k; ++j) {
            cin >> from;
            --from;
            incoming[i].push_back(from);
            in_edge[from].push_back(i);
        }
    }

    vector<int> valid_edge(M, 1), par(N, -1);

    auto bfs = [&]() {
        queue<int> q;
        vector<int> rem(M), done(N);
        for (int i = 0; i < M; ++i) rem[i] = incoming[i].size();
        vector<int> order;
        for (int i = 0; i < N; ++i) {
            if (is_source[i]) {
                q.push(i);
                done[i] = 1;
            }
        }

        while (!q.empty()) {
            int at = q.front();
            q.pop();
            if (at == T) return order;
            for (int i : in_edge[at]) {
                if (!valid_edge[i]) continue;
                if (done[outgoing[i]]) continue;
                if (!--rem[i]) {
                    order.push_back(i);
                    q.push(outgoing[i]);
                    done[outgoing[i]] = 1;
                    par[outgoing[i]] = i;
                }
            }
        }
        return vector<int>{-1};
    };

    auto order = bfs();
    if (order.size() && order[0] == -1) {
        cout << -1 << endl;
        return 0;
    }

    if (order.empty()) {
        cout << 0 << endl;
        return 0;
    }

    for (int i = 0; i < M; ++i) {
        valid_edge[i] = 0;
        order = bfs();
        if (order.size() && order[0] == -1)
            valid_edge[i] = 1;
    }
    auto ans = bfs();
    cout << ans.size() << endl;
    for (int i : ans) cout << i+1 << " ";
    cout << endl;
}
