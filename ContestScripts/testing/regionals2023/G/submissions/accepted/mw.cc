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
        for (int j = 0; j < A; j++)
            cin >> C[i][j];

    vector<vector<int64_t>> dp_in(N+1, vector<int64_t>(N+1, 1));
    auto dp_out_l = dp_in, dp_out_r = dp_in;

    for (int i = N-1; i >= 0; --i) {
        for (int j = i; j < N; ++j) {
            dp_in[i][j] = 0;
            if ((j-i+1)%2) continue;
            for (int k = i+1; k <= j; ++k) {
                dp_in[i][j] += mp(dp_in[i+1][k-1], dp_in[k+1][j], C[S[i]-'A'][S[k]-'A']);
                dp_in[i][j] %= MOD;
            }
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = N-1; j > i; --j) {
            dp_out_l[i][j] = mp(i == 0 ? 1 : dp_in[0][i-1], dp_in[j+1][N-1]);
            for (int k = 0; k < i; ++k) {
                dp_out_l[i][j] += mp(dp_in[k+1][i-1], dp_out_r[k][j]);
                dp_out_l[i][j] %= MOD;
            }

            dp_out_r[i][j] = 0;
            for (int k = j+1; k < N; ++k) {
                dp_out_r[i][j] += mp(dp_in[j+1][k-1], dp_out_l[i][k], C[S[i]-'A'][S[k]-'A']);
                dp_out_r[i][j] %= MOD;
            }
        }
    }


    for (int i = 0; i < N; ++i) {
        for (int c = 0; c < A; ++c) {
            char oc = S[i];
            S[i] = c+'A';
            int64_t ans = 0;
            for (int j = 0; j < N; ++j) {
                if (j == i) continue;
                int l = min(i,j), r = max(i, j);
                ans += mp(dp_in[l+1][r-1], dp_out_l[l][r], C[S[l]-'A'][S[r]-'A']);
                ans %= MOD;
            }
            cout << ans << endl;
            S[i] = oc;
        }
    }
}
