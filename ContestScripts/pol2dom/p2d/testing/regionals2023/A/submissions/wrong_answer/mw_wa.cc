#include <bits/stdc++.h>
using namespace std;

int main() {
    int64_t N, K;
    cin >> N >> K;
    vector<int64_t> a(N);
    for (int i = 0; i < N; ++i) cin >> a[i];
    int fw = -1, fl;
    int len = 0;
    for (int i = N-1; i >= 0; --i) {
        ++len;
        if (a[i]) {
            bool fwsingle = len%2 ? a[i]%(K+1) != 0 : 0;
            bool flsingle = len%2 ? a[i]%(K+1) != 1 : 1;
            if (fw == -1) {
                fw = fwsingle;
                fl = flsingle;
            } else {
                fw = fwsingle && !fw || flsingle && fw;
                fl = fwsingle && !fl || flsingle && fl;
            }
            len = 0;
        }
    }

    if (fw) {
        cout << "Aaron" << endl;
    } else {
        cout << "Bertha" << endl;
    }
}
