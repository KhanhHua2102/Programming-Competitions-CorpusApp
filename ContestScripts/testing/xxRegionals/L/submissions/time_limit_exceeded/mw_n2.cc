#include <bits/stdc++.h>
using namespace std;

int64_t brute(vector<int>& v, vector<int>& w, int cap) {
    int n = v.size();
    vector<int64_t> dp(n+1, 0);
    for (int i = n-1; i >= 0; --i) {
        int64_t mx = v[i], ws = w[i], ans = dp[i+1]+mx;
        for (int j = i+1; j < n; ++j) {
            mx = max(mx, (int64_t)v[j]);
            ws += w[j];
            if (ws > cap) break;
            ans = min(ans, dp[j+1]+mx);
        }
        dp[i] = ans;
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
