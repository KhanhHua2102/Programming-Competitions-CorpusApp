// O(U * K^2)

#include <bits/stdc++.h>
using namespace std;

const int MODD = 1000000007;
const int MAX_K = 1 << 14;
int64_t A[2][MAX_K];
int64_t B[2 * MAX_K];

int64_t add_mod(int64_t x, int64_t y) { return (x + y) % MODD; }

int main() {
  int n, k, L, U;
  cin >> n >> k >> L >> U;

  for (int i = 0; i < n; i++) {
    int x;
    cin >> x;
    B[x % k]++;
  }
  reverse(B + 1, B + k);
  copy(B, B + k, B + k);

  int64_t ans = 0;
  A[0][0] = 1;
  for (int r = 0, d = 0; r <= U; r++, d = 1 - d) {
    if (r >= L) ans += A[d][0];
    for (int i = 0; i < k; i++)
      A[1 - d][i] = inner_product(A[d], A[d] + k, B + (k - i), 0LL, add_mod,
                                  std::multiplies<int64_t>());
  }

  cout << ans % MODD << endl;
}
