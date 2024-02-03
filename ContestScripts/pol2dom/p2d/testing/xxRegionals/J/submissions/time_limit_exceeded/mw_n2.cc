#include <bits/stdc++.h>
using namespace std;

int N;
vector<vector<int>> al;
vector<int> dp;

int f(int at);

void dfs(int at, set<int>& s, int x) {
    for (int i : al[at]) x ^= f(i);
    if (at != 0)
        s.insert(x);
    for (int i : al[at]) {
        x ^= f(i);
        dfs(i, s, x);
        x ^= f(i);
    }
}

int f(int at) {
    if (al[at].empty()) return 1;
    if (dp[at] != -1) return dp[at];
    set<int> vals;
    dfs(at, vals, 0);
    for (int i = 0;; ++i) if (!vals.count(i)) return dp[at] = i;
}

int main() {
    cin >> N;
    al.assign(N+1, vector<int>());
    for (int i = 1; i <= N; ++i) {
        int p;
        cin >> p;
        al[p].push_back(i);
    }
    dp.assign(N+1, -1);
    cout << (f(0) ? "Jay" : "ja$on") << endl;
}
