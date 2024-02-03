#include <bits/stdc++.h>
using namespace std;

int N;
vector<int> S, M;
vector<double> C;

double go(int i) {
    if (i >= N) return 0;
    return max(go(i+1), go(i+M[i]+1)*(1-C[i]) + S[i]*C[i]);
}

int main() {
    cin >> N;
    S = M = vector<int>(N);
    C = vector<double>(N);
    for (int i = 0; i < N; ++i) cin >> S[i];
    for (int i = 0; i < N; ++i) cin >> C[i];
    for (int i = 0; i < N; ++i) cin >> M[i];

    cout << fixed << setprecision(10) << go(0) << endl;
}
