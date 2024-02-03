// O(K^U)

#include <bits/stdc++.h>
using namespace std;

const int64_t MODD = 1000000007;

// (payout, num)
vector<pair<int, int64_t>> B;

int k, L, U;

int64_t add_mod(int64_t x, int64_t y, int64_t m = MODD) { return (x + y) % m; }
int mul_mod(int64_t x, int64_t y) { return (x * y) % MODD; }

int f(int round, int curr_mod, int multiplier) {
  if (round > U) return 0;

  int64_t ans = 0;
  if (round >= L && curr_mod == 0) ans = multiplier;

  for (auto [payout, cnt] : B)
    ans = add_mod(ans, f(round + 1, add_mod(curr_mod, payout, k),
                         mul_mod(multiplier, cnt)));

  return ans;
}

int main() {
  int n;
  cin >> n >> k >> L >> U;

  map<int, int> M;
  for (int i = 0; i < n; i++) {
    int x;
    cin >> x;
    M[x % k]++;
  }
  B.assign(M.begin(), M.end());

  cout << f(0, 0, 1) << endl;
}
