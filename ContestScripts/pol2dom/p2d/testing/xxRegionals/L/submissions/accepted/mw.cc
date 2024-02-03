#include <bits/stdc++.h>
using namespace std;

int64_t fast(vector<int>& v, vector<int>& w, int cap) {
    int n = v.size();
    vector<int> gtr(n, n);
    for (int i = n-2; i >= 0; --i) {
        int j = i+1;
        while (v[j] <= v[i] && j < n) j = gtr[j];
        gtr[i] = j;
    }
    deque<pair<int, int>> ranges;
    multiset<int64_t> rvals;
    vector<int64_t> dp(n+1, 0);
    int j = n;
    int64_t sm = 0;
    for (int i = n-1; i >= 0; --i) {
        sm += w[i];
        while (j > i && sm > cap) {
            --j;
            sm -= w[j];
        }
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
        ranges.emplace_front(i, min(j-1, gtr[i]-1));
        rvals.insert(v[i]+dp[min(j, gtr[i])]);
        while (ranges.back().second >= j) {
            tie(f, s) = ranges.back();
            rvals.erase(rvals.find(v[f]+dp[s+1]));
            s = j-1;
            ranges.back().second = s;
            if (f > s)
                ranges.pop_back();
            else
                rvals.insert(v[f]+dp[s+1]);
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
