#include <bits/stdc++.h>
using namespace std;

int main() {
    int N, init, days;
    cin >> N >> init >> days;
    multiset<int64_t, greater<int64_t>> B;
    for (int i = 0; i < N; ++i) {
        int b;
        cin >> b;
        B.insert(b);
    }

    int64_t sz = init;
    for (int d = 0; d < days; ++d) {
        auto it = B.upper_bound(sz);
        if (it == end(B)) break;
        sz += (int)ceil(sqrt(*it));
        B.erase(it);
    }
    cout << sz << endl;

}
