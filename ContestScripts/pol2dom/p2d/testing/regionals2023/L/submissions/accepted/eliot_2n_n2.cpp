#include <bits/stdc++.h>

int main() {
  std::ios::sync_with_stdio(false);
  int N, M;
  std::cin >> N >> M;

  std::vector<std::vector<int>> adj(N);
  for (int i = 0; i < M; ++i) {
    int a, b;
    std::cin >> a >> b;
    --a;
    --b;
    adj[a].push_back(b);
    adj[b].push_back(a);
  }

  int best = 0;
  for (int i = 0; i < (1 << N); ++i) {
    int val = 0;
    for (int j = 0; j < N; ++j)
      if (i & (1 << j))
        for (int k : adj[j]) val += !(i & (1 << k));

    best = std::max(best, val);
  }

  std::cout << best << std::endl;
}
