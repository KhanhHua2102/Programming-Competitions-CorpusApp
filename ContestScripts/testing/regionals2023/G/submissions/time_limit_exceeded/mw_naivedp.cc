#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9+7;
int C[26][26];

int64_t mp(int64_t n) { return n % MOD; }

template <typename... Args>
int64_t mp(int64_t n, Args... args) {
    return (n * mp(args...)) % MOD;
}

int main() {
    int N, A;
    string S;
    cin >> N >> A >> S;
    for (int i = 0; i < A; ++i)
        for (int j = 0; j < A; ++j)
            cin >> C[i][j];

    vector<vector<int64_t>> dp;
    auto fill = [&]() {
        dp.assign(N+1, vector<int64_t>(N, 1));
        
        for (int i = N-1; i >= 0; --i) {
            for (int j = i; j < N; ++j) {
                dp[i][j] = 0;
                if ((j-i+1)%2) continue;
                for (int k = i+1; k <= j; ++k) {
                    dp[i][j] += mp(dp[i+1][k-1], dp[k+1][j], C[S[i]-'A'][S[k]-'A']);
                    dp[i][j] %= MOD;
                }
            }
        }
        return dp[0][N-1];
    };

    int64_t orig = fill();

    for (int i = 0; i < N; ++i) {
        for (int c = 0; c < A; ++c) {
            if (S[i]-'A' == c) {
                cout << orig << endl;
                continue;
            }
            char orig_c = S[i];
            S[i] = 'A'+c;
            cout << fill() << endl;
            S[i] = orig_c;
        }
    }
}
