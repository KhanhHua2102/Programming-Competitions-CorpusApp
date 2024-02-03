// The naive DP but it skips over endpoints that don't increase the maximum.
#include <bits/stdc++.h>
using namespace std;

int64_t brute(vector<int>& v, vector<int>& w, int cap) {
    int n = v.size();
    vector<int> gtr(n, n);
    for (int i = n-2; i >= 0; --i) {
        int j = i+1;
        while (v[j] <= v[i] && j < n) j = gtr[j];
        gtr[i] = j;
    }
    vector<int64_t> dp(n+1, 0);
    int j = n;
    int64_t sumw = 0;
    for (int i = n-1; i >= 0; --i) {
        sumw += w[i];
        while (sumw > cap) {
            j--;
            sumw -= w[j];
        }
        int64_t best = 1e18;
        int idx = i;
        while (idx < j) {
            int prev = idx;
            idx = min(j, gtr[idx]);
            best = min(best, v[prev]+dp[idx]);
        }
        dp[i] = best;
    }
    return dp[0];
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n, cap;
    cin >> n >> cap;
    vector<int> v(n), w(n);
    for (int i = 0; i < n; ++i) cin >> v[i] >> w[i];
    cout << brute(v, w, cap) << endl;
}
