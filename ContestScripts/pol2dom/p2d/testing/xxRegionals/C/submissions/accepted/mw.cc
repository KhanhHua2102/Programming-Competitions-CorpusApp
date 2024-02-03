#include <bits/stdc++.h>
using namespace std;

int main() {
    int N;
    cin >> N;
    vector<int64_t> W(N), C(N), D(N);
    for (int i = 0; i < N; i++)
        cin >> W[i] >> C[i] >> D[i];
    vector<int> ids(N);
    iota(ids.begin(), ids.end(), 0);
    int64_t ans = -1;
    do {
        int64_t rc = C[ids[0]]-W[ids[0]], d = D[ids[0]];
        for (int i = 1; i < N; ++i) {
            rc = min(rc-W[ids[i]], C[ids[i]]-W[ids[i]]);
            if (rc < 0) break;
            d += D[ids[i]];
        }
        ans = max(ans, d);
    } while (next_permutation(begin(ids), end(ids)));
    cout << ans << endl;
}
