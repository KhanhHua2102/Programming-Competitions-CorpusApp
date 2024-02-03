#include <bits/stdc++.h>

#include "bounds.h"
#include "testlib.h"

using namespace std;

int main(int argc, char* argv[]) {
  registerGen(argc, argv, 1);

  int len_S = MAX_S;
  int k = len_S - 1;
  int Jsz = len_S;

  if (argc >= 2) Jsz = atoi(argv[1]);

  println(k);
  println(string(len_S, '$'));
  println(string(Jsz, '$'));
}
