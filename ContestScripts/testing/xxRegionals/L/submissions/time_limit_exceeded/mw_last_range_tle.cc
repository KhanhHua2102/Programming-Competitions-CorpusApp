// An almost correct solution that adds the whole range every time.
// This causes an issue where the right endpoint of the last range can increase if it is replaced.
#include <bits/stdc++.h>
using namespace std;

int64_t fast(vector<int>& v, vector<int>& w, int cap) {
    int n = v.size();
    vector<int64_t> cs(n);
    cs[0] = w[0];
    for (int i = 1; i < n; ++i) cs[i] = cs[i-1]+w[i];
    vector<int> gtr(n, n);
    for (int i = n-2; i >= 0; --i) {
        int j = i+1;
        while (v[j] <= v[i] && j < n) j = gtr[j];
        gtr[i] = j;
    }
    deque<pair<int, int>> ranges;
    multiset<int64_t> rvals;
    vector<int64_t> dp(n+1, 0);
    for (int i = n-1; i >= 0; --i) {
        int f, s;
        while (ranges.size() && v[ranges.front().first] <= v[i]) {
            tie(f, s) = ranges.front();
            rvals.erase(rvals.find(v[f]+dp[s+1]));
            ranges.pop_front();
        }
        if (ranges.size()) {
            tie(f, s) = ranges.front();
            rvals.erase(rvals.find(v[f]+dp[s+1]));
            ranges.pop_front();
            ranges.emplace_front(gtr[i], s);
            rvals.insert(v[gtr[i]]+dp[s+1]);
        }
        ranges.emplace_front(i, gtr[i]-1);
        rvals.insert(v[i]+dp[gtr[i]]);
        while (cs[ranges.back().second]-cs[i]+w[i] > cap) {
            tie(f, s) = ranges.back();
            rvals.erase(rvals.find(v[f]+dp[s+1]));
            ranges.back().second--;
            if (f > s) 
                ranges.pop_back();
            else
                rvals.insert(v[f]+dp[s]);
        }
        dp[i] = *rvals.begin();
    }
    return dp[0];
}

int main() {
    int n, cap;
    cin >> n >> cap;
    vector<int> v(n), w(n);
    for (int i = 0; i < n; ++i) cin >> v[i] >> w[i];
    cout << fast(v, w, cap) << endl;
}
