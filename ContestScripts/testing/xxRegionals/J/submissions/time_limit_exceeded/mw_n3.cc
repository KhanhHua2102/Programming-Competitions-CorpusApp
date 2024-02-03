#include <bits/stdc++.h>
using namespace std;

int N;
vector<vector<int>> al;
vector<int> dp, par;

int f(int at);

void dfs(int at, set<int>& s, int rt) {
    if (at != 0) {
        int x = 0;
        int curr = at, prev = -1;
        while (1) {
            for (int i : al[curr]) if (i != prev) x ^= f(i);
            if (curr == rt) break;
            prev = curr;
            curr = par[curr];
        }
        s.insert(x);
    }
    for (int i : al[at])
        dfs(i, s, rt);
}

int f(int at) {
    if (al[at].empty()) return 1;
    if (dp[at] != -1) return dp[at];
    set<int> vals;
    dfs(at, vals, at);
    for (int i = 0;; ++i) if (!vals.count(i)) return dp[at] = i;
}

int main() {
    cin >> N;
    al.assign(N+1, vector<int>());
    par.assign(N+1, -1);
    for (int i = 1; i <= N; ++i) {
        int p;
        cin >> p;
        par[i] = p;
        al[p].push_back(i);
    }
    dp.assign(N+1, -1);
    cout << (f(0) ? "Jay" : "ja$on") << endl;
}
