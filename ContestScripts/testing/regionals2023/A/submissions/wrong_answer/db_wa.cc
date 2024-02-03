#include <bits/stdc++.h>
using namespace std;

bool can_win(int bin, int64_t stones, int64_t k) {
  return bin % 2 == 0 && stones % (k + 1) != 0;
}

bool do_case() {
  int n;
  int64_t K;
  cin >> n >> K;

  vector<int64_t> B(n);
  for (int64_t& x : B) cin >> x;
  reverse(B.begin(), B.end());

  bool win = false;
  for (int i = 0; i < n; i++) {
    if (B[i] == 0) continue;
    win ^= can_win(i, B[i], K);
  }

  return win;
}

int main() { cout << (do_case() ? "Aaron" : "Bertha") << endl; }
