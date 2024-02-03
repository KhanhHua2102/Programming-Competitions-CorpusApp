#include <bits/stdc++.h>

std::vector<int64_t> nums;

int main() {
  int N;
  std::cin >> N;
  int64_t sum = 0;
  for (int i = 0; i < N; i++) {
    int64_t x;
    std::cin >> x;
    nums.push_back(x);
    sum += x;
  }
  std::sort(nums.begin(), nums.end());
  int64_t best = sum - 2 * nums[0];
  for (int i = 0; i < N - 1; i++) {
    sum = sum - 2 * nums[i];
    best = std::max(best, sum);
  }
  std::cout << best << std::endl;
}
