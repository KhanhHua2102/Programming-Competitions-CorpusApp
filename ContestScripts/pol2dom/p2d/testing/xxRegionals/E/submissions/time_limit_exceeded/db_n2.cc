#include <bits/stdc++.h>
using namespace std;

int main() {
  cin.tie(0);
  ios_base::sync_with_stdio(false);

  int n;
  cin >> n;

  vector<int> A(n);
  for (int& x : A) cin >> x;

  vector<int> ans;
  ans.reserve(n - 1);
  for (int i = 1; i < n; i++) {
    int x = A[i] - i;

    int best_diff = abs((A[i] - i) - A[0]);
    int best_idx = 0;
    for (int j = 1; j < i; j++) {
      int diff = abs((A[i] - (i - j)) - A[j]);
      if (diff <= best_diff) {
        best_diff = diff;
        best_idx = j;
      }
    }

    ans.push_back(best_idx + 1);
  }

  bool first = true;
  for (int x : ans) {
    cout << (first ? "" : " ") << x;
    first = false;
  }
  cout << endl;
}
