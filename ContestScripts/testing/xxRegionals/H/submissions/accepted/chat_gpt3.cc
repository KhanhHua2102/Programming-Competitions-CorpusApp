#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>

using namespace std;

// Function declarations
vector<long long> solve(long long x, long long y);
long long gcdd(long long a, long long b);
long long modInverse(long long a, long long b);
vector<long long> eEuclid(long long a, long long b);

int main() {
  long long n;
  cin >> n;
  for (int i = 0; i < n; i++) {
    long long x0, y0;
    cin >> x0 >> y0;
    long long x1, y1;
    cin >> x1 >> y1;
    long long dx = x1 - x0;
    long long dy = y1 - y0;
    long long xSign = dx >= 0 ? 1 : -1;
    long long ySign = dy >= 0 ? 1 : -1;
    vector<long long> ans = solve(dx * xSign, dy * ySign);
    cout << x0 + ans[0] * xSign << " " << y0 + ans[1] * ySign << endl;
  }
  return 0;
}

// Function definitions
vector<long long> solve(long long x, long long y) {
  if (x > y) {
    vector<long long> tmp = solve(y, x);
    return {tmp[1], tmp[0]};
  }
  if (x == 0) return {1, y / 2};
  long long gcd = gcdd(x, y);
  if (gcd != 1) {
    vector<long long> ans = solve(x /= gcd, y /= gcd);
    long long scale = gcd / 2;
    ans[0] += x * scale;
    ans[1] += y * scale;
    return ans;
  }
  long long mi = modInverse(x, y);
  long long dx = mi * x / y, dy = mi;
  if (dy > y / 2) {
    dy = y - dy;
    dx = (dy * x + y - 1) / y;
  }
  // cerr << "(" << x << ", " << y << "): mi: " << mi << endl;
  if (mi == 0) dy++;
  return {dx, dy};
}

long long gcdd(long long a, long long b) {
  if (b == 0) return a;
  return gcdd(b, a % b);
}

long long modInverse(long long a, long long b) {
  return ((eEuclid(a, b)[1] % b) + b) % b;
}

vector<long long> eEuclid(long long a, long long b) {
  if (b == 0) return { a, 1, 0 };
  vector<long long> ans = eEuclid(b, a % b);
  long long temp = ans[1] - ans[2] * (a / b);
  ans[1] = ans[2];  ans[2] = temp;
  return ans;
}
