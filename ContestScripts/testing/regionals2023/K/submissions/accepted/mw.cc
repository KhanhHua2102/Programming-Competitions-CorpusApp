#include <bits/stdc++.h>
using namespace std;

int64_t N, K, L, U;

const int64_t MOD = 1e9+7;

int main() {
    cin >> N >> K >> L >> U;
    vector<int> b(N);
    for (int i = 0; i < N; ++i) cin >> b[i];
    vector<vector<int64_t>> ways(1, vector<int64_t>(K, 0));
    for (int i = 0; i < N; ++i) ways[0][b[i]%K]++;
    for (int p = 1; (1ll<<p) <= U; ++p) {
        vector<int64_t> nxt(K, 0);
        for (int i = 0; i < K; ++i) {
            for (int j = 0; j < K; ++j) {
                nxt[(i+j)%K] += (ways.back()[i] * ways.back()[j])% MOD;
                nxt[(i+j)%K] %= MOD;
            }
        }
        ways.push_back(nxt);
    }
    vector<vector<int64_t>> ways0(1, vector<int64_t>(K, 0));
    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < K; ++j) if ((i+j)%K == 0) {
            ways0[0][i] += ways[0][j];
            ways0[0][i] %= MOD;
        }
    }
    for (int p = 1; (1ll<<p) <= U; ++p) {
        auto nxt = ways0.back();
        for (int i = 0; i < K; ++i) {
            for (int j = 0; j < K; ++j) {
                nxt[i] += (ways[p-1][j] * ways0[p-1][(i+j)%K]) % MOD;
                nxt[i] %= MOD;
            }
        }
        ways0.push_back(nxt);
    }

    vector<int64_t> ans_ways(K+1, 0);
    ans_ways[0] = 1;
    int64_t rem = L;
    for (int p = 40; p >= 0; --p) if (rem >= (1ll<<p)) {
        rem -= (1ll<<p);
        vector<int64_t> nxt(K+1, 0);
        for (int i = 0; i < K; ++i) {
            for (int j = 0; j < K; ++j) {
                nxt[(i+j)%K] += (ans_ways[i] * ways[p][j]) % MOD;
                nxt[(i+j)%K] %= MOD;
            }
        }
        ans_ways = nxt;
    }
    rem = U-L;
    ans_ways[K] = ans_ways[0];
    for (int p = 40; p >= 0; --p) if (rem >= (1ll<<p)) {
        int64_t len = (1ll<<p);
        rem -= len;
        vector<int64_t> nxt(K+1, 0);
        nxt[K] = ans_ways[K];
        for (int i = 0; i < K; ++i) {
            nxt[K] += (ans_ways[i] * ways0[p][i]) % MOD;
            nxt[K] %= MOD;
            for (int j = 0; j < K; ++j) {
                nxt[(i+j)%K] += (ans_ways[i] * ways[p][j]) % MOD;
                nxt[(i+j)%K] %= MOD;
            }
        }
        ans_ways = nxt;
    }

    cout << ans_ways[K] << endl;
}
