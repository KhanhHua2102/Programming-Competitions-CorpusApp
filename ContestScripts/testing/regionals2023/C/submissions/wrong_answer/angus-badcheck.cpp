// O(N^2 log N) incorrect solution
// when checking if an edge i -> j exists, it only whether the line doesn't exits the polygon at i and j
// so it will include some edges that shouldn't actually exist

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

bool edge_valid(int i, int j) {
    if (!edge_end_valid(i, j) || !edge_end_valid(j, i)) return false;

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
        for (int b=0;b<n;b++)
            printf("%.8Lf ", d[b]);
        printf("\n");
    }
}
