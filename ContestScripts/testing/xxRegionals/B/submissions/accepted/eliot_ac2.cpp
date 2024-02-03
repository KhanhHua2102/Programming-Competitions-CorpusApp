#include <bits/stdc++.h>

#include <cstdint>

int main() {
  int N;
  std::cin >> N;
  int64_t l = 0;
  int64_t k = 0;
  int64_t max = INT64_MIN;
  int64_t min = INT64_MAX;
  bool needmin = true;
  bool needmax = true;
  for (int i = 0; i < N; i++) {
    int64_t x;
    std::cin >> x;
    if (x > 0) {
      l += x;
      needmax = false;
    } else if (x < 0) {
      k += x;
      needmin = false;
    } else {
      if (!needmax)
        needmax = false;
      else if (!needmin)
        needmin = false;
    }
    max = std::max(max, x);
    min = std::min(min, x);
  }
  if (needmin)
    std::cout << l - 2 * min << std::endl;
  else if (needmax)
    std::cout << 2 * max - k << std::endl;
  else
    std::cout << l - k << std::endl;
}
