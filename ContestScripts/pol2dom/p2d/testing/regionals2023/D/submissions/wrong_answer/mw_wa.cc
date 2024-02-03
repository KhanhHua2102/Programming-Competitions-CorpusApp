#include <bits/stdc++.h>
using namespace std;

int main() {
    int N, M, T;
    cin >> N >> M;
    T = N-1;
    vector<int> is_source(N, 1), outgoing(M);
    vector<vector<int>> in_edge(N), incoming(M);
    vector<int> done(N), rem(M), par(N, -1);
    queue<int> q;
    int k, to, from;
    for (int i = 0; i < M; ++i) {
        cin >> to >> k;
        --to;
        outgoing[i] = to;
        is_source[to] = 0;
        rem[i] = k;
        for (int j = 0; j < k; ++j) {
            cin >> from;
            --from;
            incoming[i].push_back(from);
            in_edge[from].push_back(i);
        }
    }

    for (int i = 0; i < N; ++i) {
        if (is_source[i]) {
            q.push(i);
            done[i] = 1;
        }
    }

    while (!q.empty()) {
        int at = q.front();
        q.pop();
        if (at == T) break;
        for (int i : in_edge[at]) {
            if (done[outgoing[i]]) continue;
            if (!--rem[i]) {
                q.push(outgoing[i]);
                done[outgoing[i]] = 1;
                par[outgoing[i]] = i;
            }
        }
    }

    if (!done[T]) {
        cout << -1 << endl;
        return 0;
    }
    
    vector<int> ans;
    q = queue<int>();
    done.assign(N, 0);
    done[T] = 1;
    q.push(T);
    while (!q.empty()) {
        int at = q.front();
        q.pop();
        if (par[at] == -1) continue;
        ans.push_back(par[at]);
        for (int i : incoming[par[at]]) {
            if (!done[i]) {
                q.push(i);
                done[i] = 1;
            }
        }
    }


    reverse(ans.begin(), ans.end());
    cout << ans.size() << endl;
    for (int i : ans) cout << i+1 << " ";
    cout << endl;


}
