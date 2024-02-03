#include <bits/stdc++.h>
using namespace std;

int main() {
    int N;
    cin >> N;
    vector<int> e(N);
    for (int& x : e) cin >> x;
    map<int, int> last{{e[0], 0}};
    for (int i = 1; i < N; ++i) {
        auto it = last.lower_bound(e[i]-i);
        if (it == last.end()) --it;
        int best = it->second;
        if (it != last.begin()) {
            auto p = prev(it);
            if (abs(p->first-(e[i]-i)) < abs(it->first-(e[i]-i)))
                best = p->second;
            else if (abs(p->first-(e[i]-i)) == abs(it->first-(e[i]-i)))
                best = max(best, p->second);
        }
        cout << best+1 << " ";
        last[e[i]-i] = i;
    }
    cout << endl;
}
