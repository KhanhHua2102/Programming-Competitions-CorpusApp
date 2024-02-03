#include <bits/stdc++.h>

std::vector<std::vector<int64_t>> dp;
std::deque<int64_t> dq;
int Q, K;

int64_t run_dp() {
  for (auto& a : dp) std::fill(a.begin(), a.end(), INT64_MIN);

  int64_t best = 0;
  for (int k = 0; k <= K; k++) {
    for (int i = 0; i < dq.size(); ++i) {
      // start here
      if (k == 0) dp[k][i] = dq[i];

      // start by skipping at the beginning
      if (k == 1) dp[k][i] = std::max(dp[k][i], int64_t(0));

      // continue from the previous, don't skip this one
      if (i > 0 && dp[k][i - 1] != INT64_MIN) dp[k][i] = std::max(dp[k][i], dp[k][i - 1] + dq[i]);

      // continue from the previous, skip this one
      if (i > 0 && k > 0) dp[k][i] = std::max(dp[k][i], dp[k - 1][i - 1]);

      // keep track of best skipped exactly K overall
      if (k == K) best = std::max(best, dp[k][i]);
    }
  }
  return best;
}

int main() {
  std::cin >> Q >> K;
  dp.resize(K + 1, std::vector<int64_t>(Q + 1, INT64_MIN));

  for (int i = 0; i < Q; i++) {
    char c;
    int64_t v;
    std::cin >> c;
    if (c == 'L') {
      std::cin >> v;
      dq.push_front(v);
    } else if (c == 'R') {
      std::cin >> v;
      dq.push_back(v);
    } else {
      std::cout << run_dp() << std::endl;
    }
  }
}
