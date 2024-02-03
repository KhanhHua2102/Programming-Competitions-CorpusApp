#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int N;
    cin >> N;
    vector<int> L(N), R(N);
    set<pair<int, int>> S;
    for (int i = 0; i < N; ++i) {
        cin >> L[i] >> R[i];
        S.emplace(R[i], i);
    }
    vector<int> ans;
    int at = S.begin()->second;
    S.erase(S.begin());
    ans.push_back(at);
    while (S.size()) {
        bool any = 0;
        for (auto& r : S) {
            if (L[r.second] <= R[at] && L[at] <= R[r.second]) {
                at = r.second;
                S.erase(r);
                ans.push_back(at);
                any = 1;
                break;
            }
        }
        if (!any) {
            cout << "NO" << endl;
            return 0;
        }
    }
    cout << "YES" << endl;
    for (auto& x : ans) cout << x+1 << " ";
    cout << endl;
}
