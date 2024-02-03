#include <bits/stdc++.h>
using namespace std;

bool can_win(int bin, int64_t stones, int64_t k) {
  return bin % 2 == 0 && stones % (k + 1) != 0;
}

bool can_lose(int bin, int64_t stones, int64_t k) {
  return bin % 2 != 0 || stones % (k + 1) != 1;
}

struct State {
  bool force_win;
  bool force_lose;
};

bool do_case() {
  int n;
  int64_t K;
  cin >> n >> K;

  vector<int64_t> B(n);
  for (int64_t& x : B) cin >> x;
  reverse(B.begin(), B.end());

  State last = {false, true};

  for (int i = 0; i < n; i++) {
    if (B[i] == 0) continue;
    State curr;
    curr.force_win = (last.force_lose && can_win(i, B[i], K)) ||
                     (last.force_win && !can_win(i, B[i], K));
    curr.force_lose = (last.force_lose && can_lose(i, B[i], K)) ||
                      (last.force_win && !can_lose(i, B[i], K));
    last = curr;
  }

  return last.force_win;
}

int main() { cout << (do_case() ? "Aaron" : "Bertha") << endl; }
