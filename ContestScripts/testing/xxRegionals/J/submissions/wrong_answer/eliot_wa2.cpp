#include <bits/stdc++.h>

int N;
std::vector<int> parent;
std::vector<std::vector<int>> children;
std::vector<int> roots;
std::vector<int> grunts;

int grunty(int n);

int biggrunt(int n) {
  int grunt = 0;
  for (int c : children[n]) {
    grunt ^= grunty(c);
  }
  return grunt;
}

void smexy(int n, int cur, std::vector<int>& mexes) {
  mexes.push_back(cur);
  for (int c : children[n]) {
    int newcur = cur ^ grunty(c) ^ biggrunt(c);
    smexy(c, newcur, mexes);
  }
}

int grunty(int n) {
  if (children[n].empty()) return 1;
  if (grunts[n] != -1) return grunts[n];

  // Collect all children's grunts.
  int bg = biggrunt(n);

  // Collect mexes.
  std::vector<int> mexes;
  smexy(n, bg, mexes);

  // Compute mex.
  int mex = 0;
  std::sort(mexes.begin(), mexes.end());
  for (int m : mexes) {
    if (mex == m)
      mex++;
    else
      break;
  }

  grunts[n] = mex;
  return mex;
}

int main() {
  std::cin >> N;
  parent.resize(N);
  children.resize(N);
  grunts.resize(N, -1);
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
    int grunt = grunty(root);
    ans ^= grunt;
  }
  if (ans)
    std::cout << "Jay" << std::endl;
  else
    std::cout << "ja$on" << std::endl;
}
