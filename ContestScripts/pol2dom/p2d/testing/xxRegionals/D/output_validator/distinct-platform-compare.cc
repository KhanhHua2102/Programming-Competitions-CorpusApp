#include <bits/stdc++.h>

#include "bounds.h"
#include "testlib.h"

using namespace std;

int n;
vector<int> ls;
vector<int> rs;

bool readAndCheckAnswer(InStream& in) {
  string tmp = upperCase(in.readWord("[yY][eE][sS]|[nN][oO]", "YesNo"));
  if (tmp == "NO") return false;
  set<int> used;
  int cur, l, r;
  for (int i = 0; i < n; i++) {
    int x = in.readInt(1, n, "X");
    if (used.count(x)) in.quitf(_wa, "%d was already used", x);
    int nl = ls[x - 1];
    int nr = rs[x - 1];
    if (i > 0 && (nl > r || nr < l))
      in.quitf(
          _wa,
          "Platform %d [%d, %d] does not overlap with platform %d [%d, %d]",
          cur, l, r, x, nl, nr);
    cur = x;
    l = nl;
    r = nr;
  }
  return true;
}

int main(int argc, char* argv[]) {
  registerTestlibCmd(argc, argv);

  n = inf.readInt();
  for (int i = 0; i < n; i++) {
    ls.push_back(inf.readInt());
    rs.push_back(inf.readInt());
  }
  bool ja = readAndCheckAnswer(ans);
  bool pa = readAndCheckAnswer(ouf);

  if (ja && !pa) quitf(_wa, "Jury has the answer but participant has not");
  if (!ja && pa) quitf(_fail, "Participant has the answer but jury has not");

  quitf(_ok, "Possible=%s", ja ? "YES" : "NO");
}
