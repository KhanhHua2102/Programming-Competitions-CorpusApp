#include <bits/stdc++.h>

int N;
std::vector<int> parent;
std::vector<std::vector<int>> children;
std::vector<int> roots;

std::pair<int, int> grunty(int n) {
  if (children[n].empty()) return {1, 0};
  std::vector<std::pair<int, int>> grunter;

  int allc = 0;
  for (int c : children[n]) {
    grunter.push_back(grunty(c));
    allc ^= grunter.back().first;
  }

  std::vector<int> mexes;
  mexes.push_back(allc);
  for (auto [g, c] : grunter) mexes.push_back(g ^ allc ^ c);

  int mex = 0;
  std::sort(mexes.begin(), mexes.end());
  for (int m : mexes) {
    if (mex == m)
      mex++;
    else
      break;
  }

  return {mex, allc};
}

int main() {
  std::cin >> N;
  parent.resize(N);
  children.resize(N);
  for (int i = 0; i < N; ++i) {
    std::cin >> parent[i];
    parent[i]--;
    if (parent[i] == -1) {
      roots.push_back(i);
      continue;
    }
    children[parent[i]].push_back(i);
  }
  int ans = 0;
  for (int root : roots) {
    int grunt = grunty(root).first;
    ans ^= grunt;
  }
  if (ans)
    std::cout << "Jay" << std::endl;
  else
    std::cout << "ja$on" << std::endl;
}
