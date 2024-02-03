#include <bits/stdc++.h>
using namespace std;

vector<int> zalgorithm(const string &s) {
    int n = s.size();
    vector<int> z(n);
    int L = 0, R = 0;
    for (int i = 1; i < n; i++) {
        if (i > R) {
            L = R = i;
            while (R < n && s[R-L] == s[R]) R++;
            z[i] = R-L; R--;
        } else {
            int k = i-L;
            if (z[k] < R-i+1) z[i] = z[k];
            else {
                L = i;
                while (R < n && s[R-L] == s[R]) R++;
                z[i] = R-L; R--;
            }
        }
    }
    z[0] = n;
    return z;
}

int main() {
    int K;
    string S, J;
    cin >> K >> S >> J;
    string SS = J;
    vector<int> idx;
    for (int k = 0; k <= K; ++k) {
        idx.push_back(SS.size());
        for (int i = 0; i < S.size(); i += k+1)
            SS.push_back(S[i]);
    }
    idx.push_back(SS.size());
    vector<int> z = zalgorithm(SS);
    for (int k = 0; k <= K; ++k) {
        int cnt = 0;
        for (int i = idx[k]; i+J.size()-1 < idx[k+1]; ++i)
            cnt += z[i] >= J.size();
        cout << cnt << endl;
    }
}
