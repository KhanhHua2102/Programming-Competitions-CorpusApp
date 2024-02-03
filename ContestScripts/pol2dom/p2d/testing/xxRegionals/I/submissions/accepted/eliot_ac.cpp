#include <bits/stdc++.h>

std::vector<std::pair<int, std::string>> movs;

int main() {
  int N;
  std::cin >> N;
  for (int i = 0; i < N; i++) {
    std::string name;
    std::cin >> name;
    int M;
    std::cin >> M;
    int samecount = 0;
    for (int j = 0; j < M; j++) {
      char c, p, s;
      std::cin >> c >> p >> s;
      int lcount = (c == 'Y') + (p == 'Y');
      int rcount = (p == 'Y') + (s == 'Y');
      samecount += (lcount == rcount);
    }
    movs.push_back({-samecount, name});
  }
  std::sort(movs.begin(), movs.end());
  std::cout << movs[0].second << std::endl;
}
