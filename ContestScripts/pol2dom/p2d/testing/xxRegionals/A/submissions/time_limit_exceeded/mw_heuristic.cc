#include <bits/stdc++.h>
using namespace std;

int count(const string& a, const string& b) {
    int cnt = 0;
    for (int i = 0; i+b.size()-1 < a.size(); ++i) {
        if (a[i+b.size()-1] != b.back()) continue;
        bool ok = true;
        for (int j = 0; j < b.size(); ++j) {
            if (a[i+j] != b[j]) {
                ok = false;
                break;
            }
        }
        cnt += ok;
    }
    return cnt;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int K;
    string S, J;
    cin >> K >> S >> J;
    vector<string> strs(K+1);
    for (int k = 0; k <= K; ++k)
        for (int i = 0; i < S.size(); i += k+1)
            strs[k].push_back(S[i]);
    for (int k = 0; k <= K; ++k)
        cout << count(strs[k], J) << "\n";
}
