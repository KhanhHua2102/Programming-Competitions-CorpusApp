#include <bits/stdc++.h>

std::vector<int32_t> nums;

int main() {
  int N;
  std::cin >> N;
  int32_t sum = 0;
  for (int i = 0; i < N; i++) {
    int32_t x;
    std::cin >> x;
    nums.push_back(x);
    sum += x;
  }
  std::sort(nums.begin(), nums.end());
  int32_t best = sum - 2 * nums[0];
  for (int i = 1; i < N - 1; i++) {
    sum = sum - 2 * nums[i];
    best = std::max(best, sum);
  }
  std::cout << best << std::endl;
}
