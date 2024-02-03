#include <bits/stdc++.h>
using namespace std;

const int DISTANCE = 30;

struct Point {
  int x, y;
};

Point operator-(Point P, Point Q) { return {P.x - Q.x, P.y - Q.y}; }
int64_t cross(Point P, Point Q) {
  return (int64_t)P.x * Q.y - (int64_t)P.y * Q.x;
}
int64_t dot(Point P, Point Q) {
  return (int64_t)P.x * Q.x + (int64_t)P.y * Q.y;
}
long double dist(Point P) { return sqrt(dot(P, P)); }

void TryAns(const Point& R, const Point& P, long double& best_angle,
            Point& best_point) {
  // cout << R.x << " " << R.y << endl;
  long double c = -dot(R, P - R);
  if (cross(R, P - R) == 0) return;  // collinear
  long double angle = acos(c / (dist(R) * dist(P - R)));
  if (angle <= best_angle) return;
  // cout << "Updating best " << angle << " " << c / (dist(R) * dist(P - R))
  //      << endl;
  best_point = R;
  best_angle = angle;
}

Point solve(int x, int y) {
  if (abs(x) > abs(y)) {
    Point ans = solve(y, x);
    swap(ans.x, ans.y);
    return ans;
  }

  if (x == 0) return {1, y / 2};

  long double slope = (long double)y / x;

  Point P = {x, y};

  int X = x / 2 - DISTANCE;
  long double Y = slope * X;
  long double best_angle = -5;
  Point best_P;
  for (int iters = 0; iters <= 2 * DISTANCE + 1; iters++) {
    TryAns(Point({X, Y - 1}), P, best_angle, best_P);
    TryAns(Point({X, Y}), P, best_angle, best_P);
    TryAns(Point({X, Y + 1}), P, best_angle, best_P);

    X++;
    Y += slope;
  }
  return best_P;
}

Point do_case() {
  int px, py, qx, qy;
  cin >> px >> py >> qx >> qy;

  int x = px - qx;
  int y = py - qy;

  Point ans = solve(x, y);
  ans.x += qx;
  ans.y += qy;

  return ans;
}

int main() {
  // cin.tie(0);
  // ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while (T--) {
    auto [x, y] = do_case();
    cout << x << " " << y << "\n";
  }
}
