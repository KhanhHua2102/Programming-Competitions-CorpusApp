#include "bounds.h"
#include "testlib.h"

int main(int argc, char* argv[]) {
  registerValidation(argc, argv);

  int K = inf.readInt(0, MAX_S - 1, "K");
  inf.readEoln();

  std::string S = inf.readToken("[a-z$]*", "S");
  inf.readEoln();

  for (int i = 0; i < S.size(); ++i) {
    ensuref(S[i] >= 'a' && S[i] <= 'r' || S[i] >= 't' && S[i] <= 'z' || S[i] == '$',
        "S must contain only characters in [a-r] or [t-z] or $");
  }

  std::string J = inf.readToken("[a-z$]*", "J");
  inf.readEoln();

  for (int i = 0; i < J.size(); ++i) {
    ensuref(J[i] >= 'a' && J[i] <= 'r' || J[i] >= 't' && J[i] <= 'z' || J[i] == '$',
        "J must contain only characters in [a-r] or [t-z] or $");
  }

  ensuref(J.size() <= S.size(), "J must be no longer than S");
  ensuref(K < S.size(), "K must be smaller than S");

  inf.readEof();
}
