#include <bits/stdc++.h>

typedef double dbl;
int N;
std::vector<int> S, M;
std::vector<dbl> C;
std::vector<dbl> cache;

dbl dp(int i) {
  if (i >= N) return 0.0;
  if (cache[i] >= 0.0) return cache[i];
  dbl ans = 0.0;
  // Case: skip this accountant:
  ans = std::max(ans, dp(i + 1));
  // Case: choose this accountant, reject:
  dbl reject = dp(i + M[i] + 1);
  // Case: choose this accountant, accept:
  dbl accept = S[i];
  ans = std::max(ans, C[i] * accept + (1.0 - C[i]) * reject);
  cache[i] = ans;
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin >> N;
  S.resize(N);
  M.resize(N);
  C.resize(N);
  cache.resize(N, -1);
  for (int i = 0; i < N; ++i) std::cin >> S[i];
  for (int i = 0; i < N; ++i) std::cin >> C[i];
  for (int i = 0; i < N; ++i) std::cin >> M[i];
  std::cout << std::fixed << std::setprecision(10) << dp(0) << std::endl;
}
