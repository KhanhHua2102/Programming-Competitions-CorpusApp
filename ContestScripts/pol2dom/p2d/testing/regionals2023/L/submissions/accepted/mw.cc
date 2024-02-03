#include <bits/stdc++.h>
using namespace std;

int main() {
    int N, M;
    cin >> N >> M;
    vector<vector<int>> adj(N, vector<int>(N));
    for (int i = 0; i < M; ++i) {
        int a, b;
        cin >> a >> b;
        --a; --b;
        adj[a][b] = 1;
    }
    int ans = 0;
    for (int bs = 0; bs < (1<<N); ++bs) {
        int del = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = i+1; j < N; ++j) {
                if (bool(bs&(1<<i)) == bool(bs&(1<<j))) continue;
                if (adj[i][j]) ++del;
            }
        }
        ans = max(ans, del);
    }
    cout << ans << endl;
}
