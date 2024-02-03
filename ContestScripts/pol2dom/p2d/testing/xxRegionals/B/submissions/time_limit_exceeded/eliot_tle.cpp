#include <bits/stdc++.h>

std::vector<int64_t> nums;

int main() {
  int N;
  std::cin >> N;
  for (int i = 0; i < N; i++) {
    int64_t x;
    std::cin >> x;
    nums.push_back(x);
  }
  std::sort(nums.begin(), nums.end());
  int64_t best = -1e18;
  for (int i = 1; i < N; i++) {
    int64_t sum = 0;
    for (int j = 0; j < N; j++) {
      if (j < i)
        sum -= nums[j];
      else
        sum += nums[j];
    }
    best = std::max(best, sum);
  }
  std::cout << best << std::endl;
}
