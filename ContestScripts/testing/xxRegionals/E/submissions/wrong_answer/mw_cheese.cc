#include <bits/stdc++.h>
using namespace std;

int C = 1000;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int N;
    cin >> N;
    vector<int> e(N);
    for (int& x : e) cin >> x;
    for (int i = 1; i < N; ++i) {
        int bestabs = int(1e9)+5, besti = -1;
        for (int j = 1; j <= C && i-j >= 0; ++j) {
            int cur = abs((e[i]-j)-e[i-j]);
            if (cur < bestabs) {
                bestabs = cur;
                besti = i-j;
            }
        }
        cout << besti+1 << " ";
    }
    cout << endl;
}
