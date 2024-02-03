#include <bits/stdc++.h>

int64_t K, C;
std::string F, M, L;
std::string S = "SON";

char solve(int64_t i) {
  for (int64_t k = K; k > 0; --k) {
    int64_t flen = F.size() + k * S.size();
    int64_t length = ((k + 1) * F.size() + k * (k + 1) / 2 * S.size() + int64_t(M.size()) +
        (k + 1) * int64_t(L.size()));
    if (i < flen) {
      if (i < F.size()) {
        return F[i];
      } else {
        i -= F.size();
        return S[i % S.size()];
      }
    } else if (i >= length - L.size()) {
      i -= length - L.size();
      return L[i];
    } else {
      i -= flen;
    }
  }
  return (F + M + L)[i];
}

int main() {
  std::ios::sync_with_stdio(false);

  std::cin >> K >> C;
  std::cin >> F >> M >> L;

  std::cout << solve(C) << std::endl;
}
