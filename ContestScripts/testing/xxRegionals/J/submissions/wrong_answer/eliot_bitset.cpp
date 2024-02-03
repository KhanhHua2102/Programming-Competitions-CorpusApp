#include <bits/stdc++.h>

int N;
std::vector<int> parent;
std::vector<std::vector<int>> children;
std::vector<char> dp;

char dpf(int subset) {
  if (subset == 0) return 0;
  if (dp[subset] != -1) return dp[subset];
  char ans = 0;
  for (int i = 0; i < N; ++i) {
    if (subset & (1 << i)) {
      int remove = 1 << i;
      int p = i;
      while (p != -1) {
        remove |= 1 << p;
        p = parent[p];
      }
      if (dpf(subset & (~remove)) == 0) {
        ans = 1;
        break;
      }
    }
  }
  dp[subset] = ans;
  return ans;
}

int main() {
  std::cin >> N;
  parent.resize(N);
  children.resize(N);
  dp.resize(1 << N, -1);
  for (int i = 0; i < N; ++i) {
    std::cin >> parent[i];
    parent[i]--;
    if (parent[i] == -1) continue;
    children[parent[i]].push_back(i);
  }
  if (dpf((1 << N) - 1))
    std::cout << "Jay" << std::endl;
  else
    std::cout << "ja$on" << std::endl;
}
