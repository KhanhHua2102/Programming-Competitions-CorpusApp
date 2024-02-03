#include <bits/stdc++.h>
using namespace std;

int N;
vector<int> x, y, dp;

bool closer(int i, int j) {
    int64_t sdi = (x[i]-x[N-1])*(x[i]-x[N-1]) + (y[i]-y[N-1])*(y[i]-y[N-1]);
    int64_t sdj = (x[j]-x[N-1])*(x[j]-x[N-1]) + (y[j]-y[N-1])*(y[j]-y[N-1]);
    return sdi < sdj;
}

int go(int at) {
    if (at == N-1) return 0;
    if (dp[at] != -1) return dp[at];
    int mx = 0;
    for (int nx = 1; nx < N-1; ++nx) {
        if (closer(nx, at)) {
            mx = max(mx, go(nx) + 1);
        }
    }
    return dp[at] = mx;
}

int main() {
    cin >> N;
    x = y = vector<int>(N);
    for (int i = 0; i < N; ++i) cin >> x[i] >> y[i];

    dp = vector<int>(N, -1);
    cout << go(0) << endl;
}
