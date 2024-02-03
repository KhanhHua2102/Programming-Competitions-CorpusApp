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
    auto d_order = [&](int i, int j) { return D[i] > D[j]; };
    auto w_order = [&](int i, int j) { return W[i] < W[j]; };
    auto c_order = [&](int i, int j) { return C[i] > C[j]; };
    auto dw_order = [&](int i, int j) { return (double)D[i]/W[j] > (double)D[j]/W[i]; };
    auto wc_order = [&](int i, int j) { return (double)C[i]/W[j] > (double)C[j]/W[i]; };
    auto rc_order = [&](int i, int j) { return C[i]-W[i] > C[j]-W[j]; };
    auto dc_order = [&](int i, int j) { return D[i]*C[j] > D[j]*C[i]; };
    auto dwc_order = [&](int i, int j) { return (double)D[i]/W[j]*C[j] > (double)D[j]/W[i]*C[i]; };

    vector<function<bool(int,int)>> orders = {d_order, w_order, c_order, dw_order, dc_order, dwc_order};

    int64_t ans = -1;
    for (auto order : orders) {
        sort(ids.begin(), ids.end(), order);
        int64_t rc = C[ids[0]]-W[ids[0]], d = D[ids[0]];
        for (int i = 1; i < N; ++i) {
            rc = min(rc-W[ids[i]], C[ids[i]]-W[ids[i]]);
            if (rc < 0) break;
            d += D[ids[i]];
        }
        ans = max(ans, d);
    }
    cout << ans << endl;
}
