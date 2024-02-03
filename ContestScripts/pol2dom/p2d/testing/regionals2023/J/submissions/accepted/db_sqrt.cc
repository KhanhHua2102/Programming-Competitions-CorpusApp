// Strips off A(son)^k one at a time.

#include <bits/stdc++.h>
using namespace std;

char do_case() {
  int k;
  int64_t C;
  cin >> k >> C;

  string f, m, l;
  cin >> f >> m >> l;

  for (int kk = k; kk >= 0; kk--) {
    int64_t len = f.size() + 3 * kk;
    if (C < len) {
      if (C < f.size()) return f[C];
      return "SON"[(C - f.size()) % 3];
    }
    C -= len;
  }

  if (C < m.size()) return m[C];
  C -= m.size();

  return l[C % l.size()];
}

int main() { cout << do_case() << endl; }
