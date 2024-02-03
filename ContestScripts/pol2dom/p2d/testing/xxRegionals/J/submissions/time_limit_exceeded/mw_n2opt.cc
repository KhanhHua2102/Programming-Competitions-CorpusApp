#include <bits/stdc++.h>
using namespace std;

int N;
vector<vector<int>> al;
vector<int> dp;
vector<int> seen, seen_list;

int f(int at);

void dfs(int at, int x) {
    for (int i : al[at]) x ^= f(i);
    if (at != 0) {
        seen[x] = 1;
        seen_list.push_back(x);
    }
    for (int i : al[at]) {
        x ^= f(i);
        dfs(i, x);
        x ^= f(i);
    }
}

int f(int at) {
    if (al[at].empty()) return 1;
    if (dp[at] != -1) return dp[at];
    seen_list.clear();
    dfs(at, 0);
    for (int i = 0;; ++i) if (!seen[i]) {
        for (int x : seen_list) seen[x] = 0;
        return dp[at] = i;
    }
}

int main() {
    cin >> N;
    al.assign(N+1, vector<int>());
    seen.assign(N+1, 0);
    for (int i = 1; i <= N; ++i) {
        int p;
        cin >> p;
        al[p].push_back(i);
    }
    dp.assign(N+1, -1);
    cout << (f(0) ? "Jay" : "ja$on") << endl;
}
