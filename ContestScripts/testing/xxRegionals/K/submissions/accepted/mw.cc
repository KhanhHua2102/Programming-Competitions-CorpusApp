#include <bits/stdc++.h>
using namespace std;

int main() {
    int Q, K;
    cin >> Q >> K;
    vector<int64_t> L(K+1, -1e18), R(K+1, -1e18);
    L[0] = R[0] = 0;
    int64_t tot = 0, best = 0;
    multiset<int> mink;
    for (int i = 0; i < Q; ++i) {
        string c;
        cin >> c;
        if (c == "M") {
            cout << best << endl;
            continue;
        }
        int64_t e;
        cin >> e;
        tot += e;
        mink.insert(e);
        if (mink.size() > K)
            mink.erase(prev(mink.end()));
        auto& A = (c == "L" ? L : R);
        for (int j = K; j >= 0; --j) {
            int64_t skip = j == 0 ? 0 : A[j-1];
            A[j] = max(A[j]+e, skip);
        }
        int64_t mk = 0;
        auto it = mink.begin();
        for (int j = 0; j <= K; ++j) {
            L[j] = max(L[j], tot - mk);
            R[j] = max(R[j], tot - mk);
            if (it != mink.end()) {
                mk += *it;
                ++it;
            } else break;
        }
        best = max({best, L[K], R[K]});
    }
}
