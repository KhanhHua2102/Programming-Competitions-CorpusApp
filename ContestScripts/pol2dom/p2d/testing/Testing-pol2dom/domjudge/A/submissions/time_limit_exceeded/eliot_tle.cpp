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
    // Find number of all occurrences of J in s.
    int index = 0;
    int total = 0;
    while (true) {
      index = s.find(J, index);
      if (index == std::string::npos) break;
      ++total;
      ++index;
    }
    std::cout << total << std::endl;
  }
}
