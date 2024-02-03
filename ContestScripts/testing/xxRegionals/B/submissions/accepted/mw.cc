#include <bits/stdc++.h>
using namespace std;

int main() {
    int N;
    cin >> N;
    vector<int64_t> A(N);
    int pc = 0, nc = 0;
    for (int i = 0; i < N; ++i) {
        cin >> A[i];
        if (A[i] >= 0) ++pc;
        else if (A[i] < 0) ++nc;
    }
    if (nc && pc) {
        int64_t a = 0, b = 0;
        for (int i = 0; i < N; ++i) {
            if (A[i] >= 0) a += A[i];
            else b += A[i];
        }
        cout << a-b << endl;
    } else if (pc) {
        sort(A.begin(), A.end());
        int64_t a = 0, b = A[0];
        for (int i = 1; i < N; ++i)
            a += A[i];
        cout << a-b << endl;
    } else {
        sort(A.begin(), A.end());
        int64_t a = A[N-1], b = 0;
        for (int i = 0; i < N-1; ++i)
            b += A[i];
        cout << a-b << endl;
    }
}
