// Usage: "rand_gen <rand data> <min S> <max S> <min J> <max J> <alpha>"
#include <bits/stdc++.h>

#include "bounds.h"
#include "testlib.h"

int main(int argc, char* argv[]) {
  registerGen(argc, argv, 1);
  // Testlib needs shuffling.
  for (int i = 0; i < 1000; i++) rnd.next(1, 10000234);

  int min_s = MIN_S;
  int max_s = MAX_S;
  int min_j = MIN_S;
  int max_j = MAX_S;
  int alpha = 26;

  if (argc >= 3) min_s = atoi(argv[2]);
  if (argc >= 4) max_s = atoi(argv[3]);
  if (argc >= 5) min_j = atoi(argv[4]);
  if (argc >= 6) max_j = atoi(argv[5]);
  if (argc >= 7) alpha = atoi(argv[6]);

  int S_len = rnd.next(min_s, max_s);
  std::string S;
  for (int i = 0; i < S_len; i++) S += 'a' + rnd.next(alpha);

  int J_len = rnd.next(std::min(min_j, S_len), std::min(max_j, S_len));
  std::string J;
  for (int i = 0; i < J_len; i++) J += 'a' + rnd.next(alpha);

  int K = rnd.next(0, S_len - 1);
  println(K);
  println(S);
  println(J);
}
