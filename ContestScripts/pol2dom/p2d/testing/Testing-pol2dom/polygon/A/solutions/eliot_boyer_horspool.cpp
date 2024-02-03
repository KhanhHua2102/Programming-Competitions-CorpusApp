#include <bits/stdc++.h>

std::string jason(const std::string& s, int k) {
  k++;
  std::string j;
  for (int i = 0; i < s.size(); i += k) j += s[i];
  return j;
}

int main() {
  std::ios::sync_with_stdio(false);
  int K;
  std::string S, J;

  std::cin >> K;
  std::cin >> S;
  std::cin >> J;

  bool done = false;
  auto searcher = std::boyer_moore_horspool_searcher(J.begin(), J.end());
  for (int k = 0; k <= K; ++k) {
    if (done) {
      std::cout << 0 << std::endl;
      continue;
    }
    auto s = jason(S, k);
    if (s.size() < J.size()) {
      std::cout << 0 << std::endl;
      done = true;
      continue;
    }
    int total = 0;
    auto st = s.begin();
    while (st != s.end()) {
      auto it = std::search(st, s.end(), searcher);
      if (it == s.end()) break;
      ++total;
      st = it + 1;
    }
    std::cout << total << std::endl;
  }
}
