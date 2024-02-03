#include <bits/stdc++.h>

std::deque<int64_t> dq;
int Q, K;

int64_t answer() {
  int64_t best = 0;
  if (dq.size() <= K) return 0;

  std::vector<int64_t> vals;
  for (int st = 0; st < dq.size() - K; ++st) {
    vals.clear();
    for (int i = st; i < dq.size(); ++i) {
      vals.push_back(dq[i]);
      if (vals.size() < K) continue;
      std::sort(vals.begin(), vals.end());
      best = std::max(best, std::accumulate(vals.begin() + K, vals.end(), int64_t(0)));
    }
  }

  return best;
}

int main() {
  std::cin >> Q >> K;

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
      std::cout << answer() << std::endl;
    }
  }
}
