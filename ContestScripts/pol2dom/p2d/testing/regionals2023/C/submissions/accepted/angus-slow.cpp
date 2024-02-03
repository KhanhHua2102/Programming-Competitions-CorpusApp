// O(N^3) solution

#include <bits/stdc++.h>
using namespace std;

#define MAXN 2010
typedef long long ll;
typedef long double ld;
int n;
ll x[MAXN], y[MAXN];

ld dis(int i, int j) {
    return sqrt(pow(x[i]-x[j], 2) + pow(y[i]-y[j], 2));
}

int pre(int i) {
    return ((i-1)+n)%n;
}
int nex(int i) {
    return (i+1)%n;
}

ll cross(int a, int b, int pivot) {
    return (x[a]-x[pivot])*(y[b]-y[pivot]) - (y[a]-y[pivot])*(x[b]-x[pivot]);
}

bool isClockwise(int a, int b, int pivot) {
    return cross(a, b, pivot) <= 0;
}

bool edge_end_valid(int i, int j) {
    // check if i->j is between pr->i and pr->j

    int pr = pre(i);
    int ne = nex(i);
    if (isClockwise(pr, ne, i)) {
        return isClockwise(pr, j, i) && isClockwise(j, ne, i);
    } else {
        return isClockwise(pr, j, i) || isClockwise(j, ne, i);
    }
}

/* begin code from kactl notes */
template <class T> int sgn(T x) { return (x > 0) - (x < 0); }
template<class T>
struct Point {
	typedef Point P;
	T x, y;
	explicit Point(T x=0, T y=0) : x(x), y(y) {}
	bool operator<(P p) const { return tie(x,y) < tie(p.x,p.y); }
	bool operator==(P p) const { return tie(x,y)==tie(p.x,p.y); }
	P operator+(P p) const { return P(x+p.x, y+p.y); }
	P operator-(P p) const { return P(x-p.x, y-p.y); }
	P operator*(T d) const { return P(x*d, y*d); }
	P operator/(T d) const { return P(x/d, y/d); }
	T dot(P p) const { return x*p.x + y*p.y; }
	T cross(P p) const { return x*p.y - y*p.x; }
	T cross(P a, P b) const { return (a-*this).cross(b-*this); }
	T dist2() const { return x*x + y*y; }
	double dist() const { return sqrt((double)dist2()); }
	// angle to x-axis in interval [-pi, pi]
	double angle() const { return atan2(y, x); }
	P unit() const { return *this/dist(); } // makes dist()=1
	P perp() const { return P(-y, x); } // rotates +90 degrees
	P normal() const { return perp().unit(); }
	// returns point rotated 'a' radians ccw around the origin
	P rotate(double a) const {
		return P(x*cos(a)-y*sin(a),x*sin(a)+y*cos(a)); }
	friend ostream& operator<<(ostream& os, P p) {
		return os << "(" << p.x << "," << p.y << ")"; }
};

template<class P> bool onSegment(P s, P e, P p) {
	return p.cross(s, e) == 0 && (s - p).dot(e - p) <= 0;
}

template<class P> vector<P> segInter(P a, P b, P c, P d) {
	auto oa = c.cross(d, a), ob = c.cross(d, b),
	     oc = a.cross(b, c), od = a.cross(b, d);
	// Checks if intersection is single non-endpoint point.
	if (sgn(oa) * sgn(ob) < 0 && sgn(oc) * sgn(od) < 0)
		return {(a * ob - b * oa) / (ob - oa)};
	set<P> s;
	if (onSegment(c, d, a)) s.insert(a);
	if (onSegment(c, d, b)) s.insert(b);
	if (onSegment(a, b, c)) s.insert(c);
	if (onSegment(a, b, d)) s.insert(d);
	return {s.begin(), s.end()};
}

/* end code from kactl notes */

bool edge_valid(int i, int j) {
    if (!edge_end_valid(i, j) || !edge_end_valid(j, i)) return false;
    
    // now check that this edge does not intersect with any edges

    Point<ll> pi(x[i], y[i]);
    Point<ll> pj(x[j], y[j]);

    for (int s = 0; s < n; s++) {
        int e = nex(s);
        if (s != i && s != j && e != i && e != j) {
            Point<ll> ps(x[s], y[s]);
            Point<ll> pe(x[e], y[e]);
            auto res = segInter(ps, pe, pi, pj);
            if (res.size()) {
                return false;
            }
        }
    }
    return true;
}

bool adj[MAXN][MAXN];
ld d[MAXN];
bool seen[MAXN];

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%lld%lld", &x[i], &y[i]);
    }
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            if (edge_valid(i, j)) {
                adj[i][j] = adj[j][i] = 1;
            }
        }
    }

    for (int a=0;a<n;a++) {
        priority_queue<pair<ld, int>, vector<pair<ld, int>>, greater<pair<ld, int>>> pq;
    
        fill(d, d+n, 1e18);
        fill(seen, seen+n, false);
        d[a] = 0;
        pq.push({0, a});
        while (pq.size()) {
            int a = pq.top().second;
            pq.pop();
            if (seen[a]) continue;
            seen[a] = true;
            for (int i = 0; i < n; i++) {
                if (adj[a][i]) {
                    ld newd = d[a] + dis(a, i);
                    if (newd < d[i]) {
                        d[i] = newd;
                        pq.push({d[i], i});
                    }
                }
            }
        }
        for(int b=0;b<n;b++)
            printf("%.8Lf ", d[b]);
        printf("\n");
    }
}
