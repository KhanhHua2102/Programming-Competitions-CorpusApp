#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int N, init, days;
    cin >> N >> init >> days;
    vector<int> B(N);
    for (int& e : B) cin >> e;
    int64_t sz = init;
    int nums = N;
    for (int d = 0; d < days; ++d) {
        int bid = -1;
        for (int i = 0; i < nums; ++i) {
            if (B[i] < sz && (bid == -1 || B[i] > B[bid])) {
                bid = i;
            }
        }
        if (bid == -1) break;
        sz += (int)ceil(sqrt(B[bid]));
        swap(B[bid], B[nums-1]);
        --nums;
    }
    cout << sz << endl;

}
