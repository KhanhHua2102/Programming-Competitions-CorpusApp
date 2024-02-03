#include <bits/stdc++.h>
using namespace std;

const int MODD = 1'000'000'007;
const int MAX_N = 1 << 11;

int64_t col[MAX_N];
int n;

int con = 0, mul = 0;

struct Matrix {
  vector<vector<int64_t>> A;
  Matrix() : A(n, vector<int64_t>(n)) {
    con++;
    for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++) A[i][j] = (i == j);
  }
  void init_col(int j) const {
    for (int i = 0; i < n; i++) col[i] = A[i][j];
  }
};

Matrix mult(const Matrix& A, const Matrix& B) {
  mul++;
  Matrix R;
  for (int j = 0; j < n; j++) {
    B.init_col(j);
    for (int i = 0; i < n; i++) {
      int64_t sum = 0;
      for (int k = 0; k < n; k++) {
        sum += A.A[i][k] * col[k];
        if (sum > (1ll << 62)) sum %= MODD;
      }
      R.A[i][j] = sum % MODD;
    }
  }
  return R;
}

Matrix f(const Matrix& X, int m) {
  if (m == 0) return Matrix();

  if (m % 2 == 0) {
    Matrix Y = mult(X, f(X, m - 1));
    for (int i = 0; i < n; i++) Y.A[i][i]++;
    return Y;
  }

  Matrix XI = X;
  for (int i = 0; i < n; i++) XI.A[i][i]++;

  return mult(XI, f(mult(X, X), m / 2));
}

int main() {
  int NN, KK;
  int64_t L, U;
  cin >> NN >> KK >> L >> U;

  vector<int> ctr(KK);
  for (int i = 0; i < NN; i++) {
    int x;
    cin >> x;
    ctr[x % KK]++;
  }

  n = KK;
  Matrix M;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) M.A[i][j] = ctr[(j - i + n) % n];

  int64_t ans = (f(M, U).A[0][0] - f(M, L - 1).A[0][0]) % MODD;
  cout << (ans + MODD) % MODD << endl;
}
