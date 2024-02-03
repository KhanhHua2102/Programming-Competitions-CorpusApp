#include <bits/stdc++.h>
using namespace std;

int64_t greedy(vector<int>& v, vector<int>& w, int cap) {
    int n = v.size();
    int64_t rc = cap, ans = 0, mx = 0;
    for (int i = 0; i < n; ++i) {
        if (w[i] > rc) {
            rc = cap;
            ans += mx;
            mx = 0;
        }
        mx = max(mx, (int64_t)v[i]);
        rc -= w[i];
    }
    return ans+mx;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n, cap;
    cin >> n >> cap;
    vector<int> v(n), w(n);
    for (int i = 0; i < n; ++i) cin >> v[i] >> w[i];
    cout << greedy(v, w, cap) << endl;
}
