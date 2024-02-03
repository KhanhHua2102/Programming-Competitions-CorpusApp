#include <bits/stdc++.h>

int64_t K, C;
std::string F, M, L;
std::string S = "SON";
int64_t fs_block_en;
int64_t f_block_en;
int64_t m_block_en;
int64_t l_block_en;

char solve(int64_t i) {
  if (i < fs_block_en) {
    // handle FS block
    int64_t lo = 0, hi = K, blockidx = 0;
    while (hi - lo > 1) {
      int64_t mid = (lo + hi) / 2;
      int64_t bidx =
          mid * F.size() + K * (K + 1) / 2 * S.size() - (K - mid) * (K - mid + 1) / 2 * S.size();
      if (bidx <= i) {
        lo = mid;
        blockidx = bidx;
      } else {
        hi = mid;
      }
    }
    i -= blockidx;
    if (i < F.size()) return F[i];
    i -= F.size();
    return S[i % S.size()];
  } else if (i < f_block_en) {
    // handle F block
    i -= fs_block_en;
    return F[i];
  } else if (i < m_block_en) {
    // handle M block
    i -= f_block_en;
    return M[i];
  } else {
    // handle L block
    i -= m_block_en;
    return L[i % L.size()];
  }
}

int main() {
  std::ios::sync_with_stdio(false);

  std::cin >> K >> C;
  std::cin >> F >> M >> L;

  // e.g. K=4: |FSSS'FSSFS|F|M|LLL'L|
  //             FS block  F M L block
  // FS block: KF + K(K+1)/2 * S
  // F block: F
  // M block: M
  // L block: (K+1)L
  fs_block_en = K * F.size() + K * (K + 1) / 2 * S.size();
  f_block_en = fs_block_en + F.size();
  m_block_en = f_block_en + M.size();
  l_block_en = m_block_en + (K + 1) * L.size();

  std::cout << solve(C) << std::endl;
}
