#include <bits/stdc++.h>
using namespace std;

const int MODD = 1'000'000'007;

vector<int64_t> mult(const vector<int64_t>& A, const vector<int64_t>& B) {
  int n = A.size();
  vector<int64_t> C = B;
  reverse(C.begin() + 1, C.end());

  vector<int64_t> R;

  for (int i = 0; i < n; i++) {
    int64_t sum = 0;
    for (int j = 0; j < n; j++) {
      sum += A[j] * C[(n + j - i) % n];
      sum %= MODD;
    }
    R.push_back(sum);
  }
  return R;
}

vector<int64_t> f(const vector<int64_t>& X, int m) {
  if (m == 0) {
    vector<int64_t> A(X.size());
    A[0]++;
    return A;
  }

  if (m % 2 == 0) {
    vector<int64_t> Y = mult(X, f(X, m - 1));
    Y[0]++;
    return Y;
  }

  vector<int64_t> XI = X;
  XI[0]++;

  return mult(XI, f(mult(X, X), m / 2));
}

int main() {
  int n, k;
  int64_t L, U;
  cin >> n >> k >> L >> U;

  vector<int64_t> A(k);
  for (int i = 0; i < n; i++) {
    int x;
    cin >> x;
    A[x % k]++;
  }

  int64_t ans = (f(A, U)[0] - f(A, L - 1)[0]) % MODD;
  cout << (ans + MODD) % MODD << endl;
}
