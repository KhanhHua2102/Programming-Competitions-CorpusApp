#include <bits/stdc++.h>
using namespace std;

void insert(vector<int>& A, int val, int64_t& sum) {
  sum += val - A.back();
  for (int i = 0; i < A.size(); i++)
    if (A[i] > val) {
      for (int j = A.size() - 1; j > i; j--) A[j] = A[j - 1];
      A[i] = val;
      return;
    }
}

template <typename Iter>
void process(Iter start, Iter end, int K, int64_t& ans) {
  if (distance(start, end) <= K) return;
  vector<int> smallest(start, start + K);
  sort(smallest.begin(), smallest.end());
  int64_t sum = accumulate(smallest.begin(), smallest.end(), 0LL);
  int64_t tot = sum;

  for (Iter it = start + K; it != end; ++it) {
    tot += *it;
    if (K && smallest.back() > *it) insert(smallest, *it, sum);
    ans = max(ans, tot - sum);
  }
}

int main() {
  cin.tie(0);
  ios_base::sync_with_stdio(false);
  int Q, K;
  cin >> Q >> K;

  int64_t ans = 0;
  deque<int> A;
  while (Q--) {
    char c;
    cin >> c;
    if (c == 'M') {
      cout << ans << "\n";
      continue;
    }

    int x;
    cin >> x;
    if (c == 'L') {
      A.push_front(x);
      process(A.begin(), A.end(), K, ans);
    } else {
      A.push_back(x);
      process(A.rbegin(), A.rend(), K, ans);
    }
  }
}
