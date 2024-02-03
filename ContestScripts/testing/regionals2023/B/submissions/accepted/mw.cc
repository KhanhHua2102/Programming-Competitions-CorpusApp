#include <bits/stdc++.h>
using namespace std;

int main() {
    int N, K;
    cin >> N >> K;
    K = N*N-K;
    if (N == 1 && K == 1) {
        cout << "POSSIBLE" << endl;
        cout << "." << endl;
        return 0;
    }
    if (K < N || K > 2*N-2) {
        cout << "IMPOSSIBLE" << endl;
        return 0;
    }
    vector<string> ans(N, string(N, '#'));
    for (int i = 0; i < N; ++i) ans[i][i] = '.';
    if (K > N) {
        ans[0][0] = '#';
        ans[0][N-1] = '.';
        ans[N-1][0] = '.';
        ans[N-1][N-1] = '#';
        for (int i = 1; i <= K-N; ++i) {
            ans[i][i] = '#';
            ans[i][N-1] = '.';
            ans[N-1][i] = '.';
        }
    }

    cout << "POSSIBLE" << endl;
    for (int i = 0; i < N; ++i) cout << ans[i] << endl;

}
