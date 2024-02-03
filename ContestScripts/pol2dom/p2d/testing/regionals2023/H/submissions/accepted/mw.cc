#include <bits/stdc++.h>
using namespace std;

int N;
vector<int> S, M;
vector<double> C, dp;

double go(int i) {
    if (i >= N) return 0;
    if (dp[i] != -1) return dp[i];
    return dp[i] = max(go(i+1), go(i+M[i]+1)*(1-C[i]) + S[i]*C[i]);
}

int main() {
    cin >> N;
    S = M = vector<int>(N);
    C.assign(N, 0);
    for (int i = 0; i < N; ++i) cin >> S[i];
    for (int i = 0; i < N; ++i) cin >> C[i];
    for (int i = 0; i < N; ++i) cin >> M[i];

    dp.assign(N, -1);
    cout << fixed << setprecision(10) << go(0) << endl;
}
