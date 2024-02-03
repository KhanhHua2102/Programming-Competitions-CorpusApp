#include <bits/stdc++.h>
using namespace std;

int N;
vector<int> S, M, consider;
vector<double> C;

double go(int i) {
    if (i >= N) return 0;
    if (!consider[i]) return go(i+1);
    return max(go(i+1), go(i+M[i]+1)*(1-C[i]) + S[i]*C[i]);
}

int main() {
    cin >> N;
    S = M = vector<int>(N);
    C = vector<double>(N);
    for (int i = 0; i < N; ++i) cin >> S[i];
    for (int i = 0; i < N; ++i) cin >> C[i];
    for (int i = 0; i < N; ++i) cin >> M[i];

    consider = vector<int>(N);
    vector<pair<int, int>> skill_order;
    for (int i = 0; i < N; ++i) skill_order.emplace_back(S[i]*C[i], i);
    sort(skill_order.begin(), skill_order.end(), greater<pair<int, int>>());
    for (int i = 0; i < min(N, 20); ++i) consider[skill_order[i].second] = 1;

    cout << fixed << setprecision(10) << go(0) << endl;
}
