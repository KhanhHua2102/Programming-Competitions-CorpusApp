// Usage: "rand_gen <rand data> <min S> <max S> <min J> <max J> <alpha>"
#include <bits/stdc++.h>
#include "bounds.h"
#include "testlib.h"

const double e = 2.7182818284590452353602874;

int main(int argc, char* argv[]) {
  registerGen(argc, argv, 1);
  // Testlib needs shuffling.
  for (int i = 0; i < 1000; i++) rnd.next(1, 10000234);

  int min_s = MIN_S;
  int max_s = MAX_S;
  int min_j = MIN_S;
  int max_j = MAX_S;
  int alpha = 26;


  int S_len = max_s;
  char c1 = 'a' + rnd.next(alpha);
  char c2 = 'a' + rnd.next(alpha);
  std::string S;
  for (int i = 0; i < S_len; i++) S += (i % 2 == 0 ? c1 : c2);

  int J_len = (int)(S_len/e);

  std::string J;
  for (int i = 0; i < J_len; i++) J += (i%2 == 0 ? c1 : c2);

  for (int i = 0; i < J_len; i++)
    if (J[i] == 's') J[i] = '$';
  for (int i = 0; i < S_len; i++)
    if (S[i] == 's') S[i] = '$';

  int K = S_len - 1;
  println(K);
  println(S);
  println(J);
}
