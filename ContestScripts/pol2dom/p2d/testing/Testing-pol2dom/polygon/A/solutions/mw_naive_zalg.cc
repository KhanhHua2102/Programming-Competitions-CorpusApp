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

int count(string& a, string& b) {
    string s = b + a;
    vector<int> z = zalgorithm(s);
    int cnt = 0;
    for (int i = b.size(); i < s.size(); ++i)
        cnt += z[i] >= b.size();
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
