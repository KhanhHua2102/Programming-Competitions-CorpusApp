#include <bits/stdc++.h>
using namespace std;

int main() {
    int64_t N, K;
    cin >> N >> K;
    vector<int64_t> a(N);
    for (int i = 0; i < N; ++i) cin >> a[i];
    int fw = -1, fl;
    for (int i = N-1; i >= 0; --i) {
        if (a[i]) {
            int len = N-i;
            bool fwsingle = len%2 ? a[i]%(K+1) != 0 : 0;
            bool flsingle = len%2 ? a[i]%(K+1) != 1 : 1;
            if (fw == -1) {
                fw = fwsingle;
                fl = flsingle;
            } else {
                bool fw_ = !fwsingle && fw || fwsingle && fl;
                fl = !flsingle && fw || flsingle && fl;
                fw = fw_;
            }
        }
    }

    if (fw) {
        cout << "Aaron" << endl;
    } else {
        cout << "Bertha" << endl;
    }
}
