#include <bits/stdc++.h>

std::vector<std::vector<int64_t>> dp1l, dp1r, dp2l, dp2r;
std::vector<int64_t> left, right, best2l, best2r;
int Q, K;

void update_dp1(std::vector<std::vector<int64_t>>& dp, std::vector<int64_t>& dq) {
  for (int k = 0; k <= K; k++) {
    int i = dq.size() - 1;
    if (k == 0) dp[k][i] = dq[i];
    if (k == 1) dp[k][i] = std::max(dp[k][i], int64_t(0));
    if (i > 0 && dp[k][i - 1] != INT64_MIN) dp[k][i] = std::max(dp[k][i], dp[k][i - 1] + dq[i]);
    if (i > 0 && k > 0) dp[k][i] = std::max(dp[k][i], dp[k - 1][i - 1]);
  }
}

void update_dp2(std::vector<std::vector<int64_t>>& dp, std::vector<int64_t>& dq) {
  for (int k = 0; k <= K; k++) {
    int i = dq.size() - 1;
    if (k == 0 && i == 0) dp[k][i] = dq[i];
    if (i == 0 && k == 1) dp[k][i] = std::max(dp[k][i], int64_t(0));
    if (i > 0 && dp[k][i - 1] != INT64_MIN) dp[k][i] = std::max(dp[k][i], dp[k][i - 1] + dq[i]);
    if (i > 0 && k > 0) dp[k][i] = std::max(dp[k][i], dp[k - 1][i - 1]);
  }
}

int main() {
  std::cin >> Q >> K;
  dp1l.resize(K + 1, std::vector<int64_t>(Q + 1, INT64_MIN));
  dp1r.resize(K + 1, std::vector<int64_t>(Q + 1, INT64_MIN));
  dp2l.resize(K + 1, std::vector<int64_t>(Q + 1, INT64_MIN));
  dp2r.resize(K + 1, std::vector<int64_t>(Q + 1, INT64_MIN));
  best2l.resize(K + 1, INT64_MIN);
  best2r.resize(K + 1, INT64_MIN);

  int64_t best1l = 0, best1r = 0;
  for (int i = 0; i < Q; i++) {
    char c;
    int64_t v;
    std::cin >> c;
    if (c == 'L') {
      std::cin >> v;
      left.push_back(v);
      update_dp1(dp1l, left);
      update_dp2(dp2l, left);
      best1l = std::max(best1l, dp1l[K][left.size() - 1]);
      for (int k = 0; k <= K; k++) best2l[k] = std::max(best2l[k], dp2l[k][left.size() - 1]);
    } else if (c == 'R') {
      std::cin >> v;
      right.push_back(v);
      update_dp1(dp1r, right);
      update_dp2(dp2r, right);
      best1r = std::max(best1r, dp1r[K][right.size() - 1]);
      for (int k = 0; k <= K; k++) best2r[k] = std::max(best2r[k], dp2r[k][right.size() - 1]);
    } else {
      auto best = std::max(best1l, best1r);
      for (int lk = 0; lk <= K; lk++) {
        int rk = K - lk;
        if (lk > left.size() || rk > right.size()) continue;
        if (right.size() == 0 && rk != 0) continue;
        if (left.size() == 0 && lk != 0) continue;
        int64_t lv = 0, rv = 0;
        if (left.size() > 0) lv = best2l[lk];
        if (right.size() > 0) rv = best2r[rk];
        if (lv == INT64_MIN || rv == INT64_MIN) continue;
        best = std::max(best, lv + rv);
      }
      std::cout << best << std::endl;
    }
  }
}
