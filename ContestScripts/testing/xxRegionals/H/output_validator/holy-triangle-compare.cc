#include <bits/stdc++.h>

#include "bounds.h"
#include "testlib.h"

using namespace std;

using Integer = __int128;
const long double PI = acosl(-1.0);

struct Point {
  Integer x, y;
};

Point operator-(Point P, Point Q) { return {P.x - Q.x, P.y - Q.y}; }
Integer cross(Point P, Point Q) { return P.x * Q.y - P.y * Q.x; }
Integer dist2(Point P, Point Q) {
  return (P.x - Q.x) * (P.x - Q.x) + (P.y - Q.y) * (P.y - Q.y);
}
long double dist(Point P, Point Q) {
  // Use hypot for no loss of precision.
  return hypotl(P.x - Q.x, P.y - Q.y);
}

// FunnyFrac is a fraction equivalent to: num / sqrt(den2)
struct FunnyFrac {
  Integer num;
  Integer den2;
};

ostream& operator<<(ostream& os, Integer x) {
  if (x <= std::numeric_limits<int64_t>::max() &&
      x >= std::numeric_limits<int64_t>::min()) {
    os << (int64_t)x;
    return os;
  }

  os << x / 10 << (int)(x % 10);
  return os;
}

ostream& operator<<(ostream& os, FunnyFrac f) {
  os << f.num << " / sqrt(" << f.den2 << ")";
  return os;
}

struct Frac {
  Integer num;
  Integer den;
};

int Compare(Frac f, Frac g) {
  if (f.num == 0 && g.num == 0) return 0;
  if (f.num == 0) return -1;
  if (g.num == 0) return 1;

  if (f.num / f.den != g.num / g.den)
    return f.num / f.den < g.num / g.den ? -1 : 1;
  if (f.num / f.den != 0)
    return Compare({f.num % f.den, f.den}, {g.num % g.den, g.den});
  return Compare({g.den, g.num}, {f.den, f.num});
}

int sign(FunnyFrac val) {
  if (val.num == 0) return 0;
  return val.num < 0 ? -1 : 1;
}

// ----------------------------------------------------------------------------

Point ReadAnswer(InStream& in, int case_num, Point P, Point Q) {
  Point R;
  // "5" is arbitrary here. Even +-1 is probably enough? The answer must be
  // close to the line. We need some value here since there can be overflow
  // without these checks which can give incorrect results.
  R.x = in.readInt(MIN_XY - 5, MAX_XY + 5, "x");
  R.y = in.readInt(MIN_XY - 5, MAX_XY + 5, "y");

  if (cross(P - Q, R - Q) == 0)
    in.quitf(_wa, "Case %d: Points are collinear.", case_num);

  return R;
}

FunnyFrac ComputeCosTheta(Point P, Point Q, Point R) {
  Integer a2 = dist2(P, R);
  Integer b2 = dist2(Q, R);
  Integer c2 = dist2(P, Q);

  // c^2        = a^2 + b^2 - 2 * a * b * cos(theta)
  // cos(theta) = -(c^2 - a^2 - b^2) / (2 * a * b)
  return {-(c2 - a2 - b2), 4 * a2 * b2};
}

enum Verdict { JUDGE_ERROR, WRONG_ANSWER, CORRECT };
Verdict CompareAnswers(FunnyFrac judge, FunnyFrac contestant) {
  // These are cos(theta) values. cos is decreasing in the range [0, 180].
  // Since we know 0 < theta < 180, we know that larger theta means smaller
  // cos(theta) value.
  if (sign(judge) != sign(contestant)) {
    return sign(judge) < sign(contestant) ? WRONG_ANSWER : JUDGE_ERROR;
  }

  // If the value is negative, negate the fraction... now we're looking for
  // large positive values. So we'll call CompareAnswer with the params
  // flipped and flip the result.
  if (sign(judge) < 0) {
    judge.num *= -1;
    contestant.num *= -1;
    Verdict value = CompareAnswers(judge, contestant);
    if (value == CORRECT) return CORRECT;
    return value == JUDGE_ERROR ? WRONG_ANSWER : JUDGE_ERROR;
  }

  // Now we have positive numbers and smaller is better.
  Frac j2 = {judge.num * judge.num, judge.den2};
  Frac c2 = {contestant.num * contestant.num, contestant.den2};

  int comp = Compare(j2, c2);

  if (comp == 0) return CORRECT;
  if (comp == -1) return WRONG_ANSWER;
  return JUDGE_ERROR;
}

Verdict CompareAnswers(Point P, Point Q, Point judgeR, Point contestantR) {
  return CompareAnswers(ComputeCosTheta(P, Q, judgeR),
                        ComputeCosTheta(P, Q, contestantR));
}

long double NaiveAngle(Point P, Point Q, Point R) {
  Integer a2 = dist2(P, R);
  Integer b2 = dist2(Q, R);
  Integer c2 = dist2(P, Q);

  long double a = dist(P, R);
  long double b = dist(Q, R);

  // c^2        = a^2 + b^2 - 2 * a * b * cos(theta)
  // cos(theta) = -(c^2 - a^2 - b^2) / (2 * a * b)
  return 180.0 / PI * acosl(-(c2 - b2 - a2) / (2 * a * b));
}

void ValidateCase(int case_num) {
  setTestCase(case_num);

  Point P, Q;
  P.x = inf.readInt();
  P.y = inf.readInt();
  Q.x = inf.readInt();
  Q.y = inf.readInt();

  Point ja = ReadAnswer(ans, case_num, P, Q);
  Point pa = ReadAnswer(ouf, case_num, P, Q);

  Verdict verdict = CompareAnswers(P, Q, ja, pa);
  if (verdict == CORRECT) return;

  // Here are the exact values:
  // cout << "Judge     : arccos(" << ComputeCosTheta(P, Q, ja) << ")" << endl;
  // cout << "Contestant: arccos(" << ComputeCosTheta(P, Q, pa) << ")" << endl;

  if (verdict == WRONG_ANSWER)
    quitf(_wa,
          "Case %d: Judge's angle (approx %.10Lf degrees) larger than "
          "contestant's angle "
          "(approx %.10Lf degrees)",
          case_num, NaiveAngle(P, Q, ja), NaiveAngle(P, Q, pa));

  quitf(_fail,
        "**ERROR** Case %d: Judge's angle (approx %.10Lf degrees) SMALLER than "
        "contestant's angle "
        "(approx %.10Lf degrees)",
        case_num, NaiveAngle(P, Q, ja), NaiveAngle(P, Q, pa));
}

int main(int argc, char* argv[]) {
  registerTestlibCmd(argc, argv);

  int T = inf.readInt();

  for (int i = 1; i <= T; i++) ValidateCase(i);

  quitf(_ok, "All %d cases correct", T);
}
