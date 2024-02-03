#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vi::iterator vit;

// strongly connected component algorithm

const int WHITE = 0;
const int GRAY = 1;
const int BLACK = 2;
const int nmax = 1 << 20;

int n, ftime, color[nmax], vertex[nmax], comp[nmax];
bool sink[nmax];
vi adj[nmax];
vi transp[nmax];

void visit(int i) {
  color[i] = GRAY;
  for (vit it = adj[i].begin() ; it != adj[i].end() ; ++it)
    if (color[*it] == WHITE)
      visit(*it);
  color[i] = BLACK;
  vertex[ftime--] = i;
}

void visit2(vi &scc, int j) {
  color[j] = GRAY;
  for (vit it = transp[j].begin() ; it != transp[j].end() ; ++it)
    if (color[*it] == WHITE)
      visit2(scc, *it);
  color[j] = BLACK;
  scc.push_back(j);
}

// returns vector of components and stores each node's component in comp
vvi components() {
  ftime = n-1;
  for (int i=0 ; i<n ; i++)
    color[i] = WHITE;
  for (int i=0 ; i<n ; i++)
    if (color[i] == WHITE)
      visit(i);
  for (int i=0 ; i<n ; i++)
    color[i] = WHITE;
  vvi sccs;
  for (int i=0 ; i<n ; i++)
    if (color[vertex[i]] == WHITE) {
      sccs.push_back(vi());
      visit2(sccs.back(), vertex[i]);
    }
  for (int i=0 ; i<(int)sccs.size() ; i++)
    for (int j=0 ; j<(int)sccs[i].size() ; j++)
      comp[sccs[i][j]] = i;
  return sccs;
}

int main() {
  cin >> n;
  for (int i=0 ; i<n ; i++) {
    int m;
    cin >> m;
    for (int j=0 ; j<m ; j++) {
      int k;
      cin >> k;
      adj[i].push_back(k-1);
      transp[k-1].push_back(i);
    }
  }
  // the solution contains all or no nodes of a strongly connected component
  vvi sccs = components();
  // if a component is in the solution, all its transitive successors are
  // find all components without successors (others cannot be optimal)
  for (int i=0 ; i<(int)sccs.size() ; i++)
    sink[i] = true;
  for (int i=0 ; i<n ; i++)
    for (vit it = adj[i].begin() ; it != adj[i].end() ; ++it)
      if (comp[i] != comp[*it])
        sink[comp[i]] = false;
  // components are disjoint
  // lexicographically smallest means the smallest identifier must be maximal
  int best=-1, smallest=-1;
  for (int i=0 ; i<(int)sccs.size() ; i++)
    if (sink[i]) {
      int s = *min_element(sccs[i].begin(), sccs[i].end());
      if (s > smallest) {
        best = i;
        smallest = s;
      }
    }
  assert(best != -1);
  sort(sccs[best].begin(), sccs[best].end());
  cout << sccs[best][0]+1;
  for (int i=1 ; i<(int)sccs[best].size() ; i++)
    cout << ' ' << sccs[best][i]+1;
  cout << endl;
  return 0;
}

