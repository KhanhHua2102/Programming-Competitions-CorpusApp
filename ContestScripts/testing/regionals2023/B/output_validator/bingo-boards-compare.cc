
#include <bits/stdc++.h>

#include "bounds.h"
#include "testlib.h"

using namespace std;

bool ReadAndCheckAnswer(InStream& in, int n, int k) {
  string poss = upperCase(in.readToken("[a-zA-Z]*", "possible_impossible"));

  if (poss == "IMPOSSIBLE") return false;
  if (poss != "POSSIBLE")
    in.quitf(_wa, "Expected to be [IM]POSSIBLE. Received: %s", poss.c_str());

  vector<string> board = in.readTokens(n, format("[.#]{%d}", n), "board");

  int total_marked = 0;
  vector<int> marked_in_row(n, 0);
  vector<int> marked_in_col(n, 0);

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      if (board[i][j] == '#') {
        total_marked++;
        marked_in_row[i]++;
        marked_in_col[j]++;
      }

  if (total_marked != k) {
    in.quitf(_wa,
             "Incorrect number of marked cells (expected: %d, received: %d).",
             k, total_marked);
  }

  for (int i = 0; i < n; i++) {
    if (marked_in_row[i] == n) in.quitf(_wa, "Row %i has all cells marked.", i);
    if (marked_in_col[i] == n)
      in.quitf(_wa, "Column %i has all cells marked.", i);
  }

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) {
      if (board[i][j] == '#') continue;

      if (marked_in_row[i] + 1 != n && marked_in_col[j] + 1 != n)
        in.quitf(_wa, "Marking (%d, %d) does not give a winning board.", i, j);
    }

  return true;
}

int main(int argc, char* argv[]) {
  registerTestlibCmd(argc, argv);

  int n = inf.readInt();
  int k = inf.readInt();

  bool judge = ReadAndCheckAnswer(ans, n, k);
  bool contestant = ReadAndCheckAnswer(ouf, n, k);

  if (!judge && contestant)
    quitf(_fail, "ERROR! Contestant found answer, but judge did not. ERROR!");

  if (judge && !contestant)
    quitf(_wa, "Judge found a board, but the contestant did not.");

  quitf(_ok, "(%sboard found)", (judge ? "" : "no "));
}
