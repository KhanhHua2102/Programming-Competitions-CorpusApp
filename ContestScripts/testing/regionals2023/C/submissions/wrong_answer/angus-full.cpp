// O(N^2 log N) solution
// For each point, we find all the points it can reach in O(N log N) with a circular sweep
// then run a dijkstras

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

vector<int> sortPointsAround(int pivot) {
    vector<int> l, r;
    for (int i = 0; i < n; i++) {
        if (i != pivot) {
            if (x[i] <= x[pivot]) l.push_back(i);
            else r.push_back(i);
        }
    }
    auto cmp = [pivot](int i, int j) {
        return cross(i, j, pivot) < 0;
    };
    sort(l.begin(), l.end(), cmp);
    sort(r.begin(), r.end(), cmp);
    l.insert(l.end(), r.begin(), r.end());
    return l;
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

template<class P>
pair<int, P> lineInter(P s1, P e1, P s2, P e2) {
    auto d = (e1 - s1).cross(e2 - s2);
    if (d == 0) // if parallel
        return {-(s1.cross(e1, s2) == 0), P(0, 0)};
    auto p = s2.cross(e1, e2), q = s2.cross(e2, s1);
    return {1, (s1 * p + e1 * q) / d};
}


/* end code from kactl notes */

bool edge_valid(int i, int j) {
    if (!edge_end_valid(i, j) || !edge_end_valid(j, i)) return false;

    return true;
}

bool noEdge[MAXN][MAXN];
ld d[MAXN];
bool seen[MAXN];

int currentCenter;
int currentSweepPoint;

struct Line {
    int i; // line from i to i+1
    ld getDistanceFromCenter() const {
        Point<ld> center(x[currentCenter], y[currentCenter]);
        Point<ld> sweepPoint(x[currentSweepPoint], y[currentSweepPoint]);
        Point<ld> pi(x[i], y[i]);
        Point<ld> pj(x[nex(i)], y[nex(i)]);
        auto inter = lineInter(center, sweepPoint, pi, pj).second;
        ld dis = sqrt(pow(inter.x-x[currentCenter], 2) + pow(inter.y-y[currentCenter], 2));
        return dis;
    }
    bool operator<(Line l) const {
        return getDistanceFromCenter() < l.getDistanceFromCenter();
    }
};

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%lld%lld", &x[i], &y[i]);
    }


    for (int at = 0; at < n; at++) {
        auto res = sortPointsAround(at);
        currentCenter = at;
        set<Line> lines;
        for (int _ = 0; _ < 2; _++) {
            // do 2 passes so that every edge is considered from start to finish
            for (auto i : res) {
                currentSweepPoint = i;
                // check whether this point is invalid
                auto it = lines.begin();
                if (it != lines.end()) {
                    if (it->getDistanceFromCenter() < dis(i, at)-1e-8) {
                        noEdge[at][i] = true;
                    }
                }

                vector<int> otherPoints;
                if (pre(i) != at) otherPoints.push_back(pre(i));
                if (nex(i) != at) otherPoints.push_back(nex(i));
                for (auto j : otherPoints) {
                    ll c = cross(i, j, at);
                    assert(cross);

                    int s = i;
                    if (j == pre(i)) s = j;

                    if (c > 0){
                        lines.erase({s});
                    }
                }

                for (auto j : otherPoints) {
                    ll c = cross(i, j, at);
                    assert(cross);

                    int s = i;
                    if (j == pre(i)) s = j;

                    if (c < 0) {
                        // line starts at i, goes to j
                        lines.insert({s});
                    }
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) noEdge[i][j] = 1;
            else if (!edge_valid(i, j)) noEdge[i][j] = 1;

            //printf("%d %d: %d\n", i+1, j+1, (int)noEdge[i][j]);
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
                if (!noEdge[a][i]) {
                    ld newd = d[a] + dis(a, i);
                    if (newd < d[i]) {
                        d[i] = newd;
                        pq.push({d[i], i});
                    }
                }
            }
        }
        
        printf("%.10Lf", d[0]);
        for (int b=1;b<n;b++)
            printf(" %.10Lf", d[b]);
        printf("\n");
    }
}
