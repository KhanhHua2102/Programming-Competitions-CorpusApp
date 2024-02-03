#include <bits/stdc++.h>

std::string jason(const std::string& s, int k) {
  k++;
  std::string j;
  for (int i = 0; i < s.size(); ++i)
    if (i % k == 0) j += s[i];
  return j;
}

std::vector<int> zalg(const std::string& p, const std::string& s) {
  std::string v = p + s;
  std::vector<int> z(v.size(), 0);
  int l = 0, r = 0;
  for (int i = 1; i < v.size(); ++i) {
    if (i > r) {
      l = r = i;
      while (r < v.size() && v[r] == v[r - l]) r++;
      z[i] = r - l;
      r--;
    } else {
      if (i + z[i - l] - 1 < r)
        z[i] = z[i - l];
      else {
        l = i;
        while (r < v.size() && v[r] == v[r - l]) r++;
        z[i] = r - l;
        r--;
      }
    }
  }
  return z;
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
    auto z = zalg(J, s);
    int total = 0;
    for (int i = J.size(); i < z.size(); ++i)
      if (z[i] >= J.size()) ++total;
    std::cout << total << std::endl;
  }
}
