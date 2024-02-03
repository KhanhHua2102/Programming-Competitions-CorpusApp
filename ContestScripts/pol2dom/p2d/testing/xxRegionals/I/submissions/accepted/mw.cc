#include <bits/stdc++.h>
using namespace std;

int main() {
    int N;
    cin >> N;
    string bn;
    int bs = -1;
    for (int i = 0; i < N; ++i) {
        string s;
        int m;
        cin >> s >> m;
        int sc = 0;
        for (int j = 0; j < m; ++j) {
            string a, b, c;
            cin >> a >> b >> c;
            if (a=="Y" && b=="N" && c=="Y") ++sc;
            else if (a==b && a==c) ++sc;
        }
        if (sc > bs) {
            bs = sc;
            bn = s;
        } else if (sc == bs) {
            bn = min(bn, s);
        }
    }
    cout << bn << endl;
}
