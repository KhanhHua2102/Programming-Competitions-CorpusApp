#include <bits/stdc++.h>
using namespace std;

int main() {
    int64_t K, C;
    cin >> K >> C;
    string f, m, l;
    cin >> f >> m >> l;

    if (C < f.size()) {
        cout << f[C] << endl;
        return 0;
    }

    if (C < f.size() + 3*K) {
        cout << "SON"[(C-f.size())%3] << endl;
        return 0;
    }

    C -= f.size() + 3*K;

    int i = 1;
    // This could be a binary search
    while (C >= f.size()+3*(K-i) && i <= K) {
        C -= f.size()+3*(K-i);
        i++;
    }

    if (i < K) {
        if (C < f.size()) {
            cout << f[C] << endl;
            return 0;
        } else {
            cout << "SON"[(C-f.size())%3] << endl;
            return 0;
        }
    }

    if (C < m.size()) {
        cout << m[C] << endl;
        return 0;
    }

    C -= m.size();

    if (C < l.size()*K) {
        cout << l[C%l.size()] << endl;
        return 0;
    }

    C -= l.size()*K;

    cout << l[C] << endl;

}
