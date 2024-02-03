#include <bits/stdc++.h>
using namespace std;

int N;
vector<int> x, y, dp;

struct xy
{
  double x,y;
//construction etc
  xy(){x=y=0;}
  xy(double X, double Y) {x = X; y = Y;}
  xy& operator=(const xy& other) {x=other.x;y=other.y; return *this;}

//vector addition
  xy operator-(const xy& other) { return xy(x-other.x,y-other.y); }
  xy& operator-=(const xy& other) {x-=other.x;y-=other.y;return *this;}
  xy operator+(const xy& other) {return xy(x+other.x,y+other.y);}
  xy& operator+=(const xy& other) {x+=other.x;y+=other.y; return *this;}

//unary vector operations
  double operator+() {return sqrt(x*x+y*y);}

//scaling, it's a good ide
  xy operator*(double a) {return xy(a*x,y*a);}
  xy& operator*=(double a) {x*=a;y*=a; return *this; }
  xy operator/(double a) {return xy(x/a, y/a);}
  xy& operator/=(double a) {x/=a;y/=a; return *this; }


//vector operations
  double operator*(const xy& other) { return x*other.x + y*other.y; }  //dot
  double operator^(const xy& other) {return (x*other.y-y*other.x);}    //cross
  xy operator~() {return xy(x/+(*this), y/+(*this)); }                 //unit
  xy operator!() {return xy(y,-x);}                                    //normal
  xy operator>>(const xy& other) {return xy(other.x-x,other.y-y);}     //this to other
  xy operator<<(const xy& other) {return xy(x-other.x,y-other.y);}     //other to this
  double distance(const xy& other) {return sqrt((x-other.x)*(x-other.x) + (y-other.y)*(y-other.y));} //distance between two points
//choice of comparison
  //lexicographic
  bool operator<(const xy& other) const
  {
    if (x < other.x) return true;
    if (x > other.x) return false;
    return y < other.y;
  }
};


//line(segment) struct

struct line {
  xy upper, lower, arrow;
  line(){}
  line(const xy& a, const xy& b)
  {
    if (a < b) {upper=a; lower=b;}
    else {upper=b; lower=a;}
    arrow = lower-upper;
  }

//closest approach point to *this line
  xy cap(xy p)
  {
    xy w = p - upper;
    double b = (w*arrow) / (arrow*arrow);
    if (0 <= b && b <= 1)
    {
      xy pb = upper + arrow * b;
      return pb;
    }
    if (p.distance(lower) < p.distance(upper)) return lower;
    return upper;
  }
};

bool ok(int i, int j) {
    xy p = line(xy(x[i], y[i]), xy(x[j], y[j])).cap(xy(x[N-1], y[N-1]));
    const double eps = 1e-8;
    return abs(p.x-x[j]) < eps && abs(p.y-y[j]) < eps;
}

int go(int at) {
    if (at == N-1) return 0;
    // if (dp[at] != -1) return dp[at];
    int mx = 0;
    for (int nx = 1; nx < N-1; ++nx) if (nx != at) {
        if (ok(at, nx)) {
            mx = max(mx, go(nx) + 1);
        }
    }
    return dp[at] = mx;
}

int main() {
    cin >> N;
    x = y = vector<int>(N);
    for (int i = 0; i < N; ++i) cin >> x[i] >> y[i];

    dp = vector<int>(N, -1);
    cout << go(0) << endl;
}
