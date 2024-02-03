#include <bits/stdc++.h>
using namespace std;

int N;
vector<int> par, dp;

bool f(int bs) {
    if (bs == 1) return 0;
    if (dp[bs] != -1) return dp[bs];
    bool g = 0;
    for (int i = 1; i < N; ++i) if (bs & (1 << i)) {
        int at = i, nbs = bs;
        while (at != 0 && (bs & (1 << at))) {
            nbs ^= (1 << at);
            at = par[at];
        }
        if (!f(nbs)) g = 1;
    }
    return dp[bs] = g;
}


int main() {
    cin >> N;
    ++N;
    par.resize(N);
    for (int i = 1; i < N; ++i) cin >> par[i];
    int bs = (1 << N) - 1;
    dp.assign(1 << N, -1);
    cout << (f(bs) ? "Jay" : "ja$on") << endl;
}
