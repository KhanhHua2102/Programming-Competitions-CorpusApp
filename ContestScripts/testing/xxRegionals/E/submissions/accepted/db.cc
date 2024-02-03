#include <bits/stdc++.h>
using namespace std;

void compare(int diff, int where, int& best_diff, int& best_where) {
  if (diff > best_diff) return;
  if (best_diff == diff && where < best_where) return;
  best_diff = diff;
  best_where = where;
}

int main() {
  int n;
  cin >> n;

  vector<int> ans;
  map<int, int> S;
  int first_val;
  cin >> first_val;
  S[first_val] = 0;
  for (int i = 1; i < n; i++) {
    int x;
    cin >> x;
    x -= i;

    int diff = abs(x - first_val);
    int where = 0;

    auto it = S.lower_bound(x);

    if (it != S.end()) compare(abs(x - it->first), it->second, diff, where);
    if (it-- != S.begin()) compare(abs(x - it->first), it->second, diff, where);

    ans.push_back(where + 1);
    S[x] = i;
  }

  bool first = true;
  for (int x : ans) {
    cout << (first ? "" : " ") << x;
    first = false;
  }
  cout << endl;
}
