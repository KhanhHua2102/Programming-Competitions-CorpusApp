#include <bits/stdc++.h>
using namespace std;

class Set {
 public:
  explicit Set(int n) {
    for (int i = 1; i <= 2 * n; i *= 2) h++;
  }

  void Insert(int x) {
    int idx = 0;
    vector<int> idxs;
    for (int i = h; i >= 0; i--) {
      idxs.push_back(idx);
      idx = (((x ^ xor_) >> i) & 1) ? Right(idx) : Left(idx);
    }

    sz_[idx] = 1;
    for (int i = idxs.size() - 1; i >= 0; i--) UpdateSize(idxs[i]);
  }

  void XOR(int x) {
    xor_ ^= x;
  }

  vector<int> Elements() const {
    if (Size() == 0) return {};
    vector<int> result;
    dfs(0, h, 0, result);

    return result;
  }

  int Size(int idx = 0) const { return idx == -1 ? 0 : sz_[idx]; }

  int Mex() const {
    int result = 0;
    int idx = 0;
    for (int i = h; i >= 0; i--) {
      if (Size(idx) == 0) break;
      auto [small, large] = children_[idx];
      if ((xor_ >> i) & 1) swap(small, large);
      if (Size(small) == (1 << i)) {
        swap(small, large);
        result |= (1 << i);
      }
      idx = small;
    }
    return result;
  }

 private:
  int h = 1;
  int xor_ = 0;
  vector<pair<int, int>> children_ = {{-1, -1}};
  vector<int> sz_ = {0};

  void UpdateSize(int idx) {
    sz_[idx] = 0;
    auto [l, r] = children_[idx];
    if (l != -1) sz_[idx] += sz_[l];
    if (r != -1) sz_[idx] += sz_[r];
  }
  int Left(int idx) {
    if (children_[idx].first == -1) {
      children_[idx].first = children_.size();
      children_.push_back({-1, -1});
      sz_.push_back(0);
    }
    return children_[idx].first;
  }
  int Right(int idx) {
    if (children_[idx].second == -1) {
      children_[idx].second = children_.size();
      children_.push_back({-1, -1});
      sz_.push_back(0);
    }
    return children_[idx].second;
  }

  void dfs(int idx, int depth, int curr, vector<int>& result) const {
    if (idx == -1) return;
    assert(sz_[idx] != 0);
    if (depth == -1) {
      result.push_back(curr);
      return;
    }

    int left = xor_ & (1 << depth);
    int right = left ^ (1 << depth);
    dfs(children_[idx].first, depth - 1, curr | left, result);
    dfs(children_[idx].second, depth - 1, curr | right, result);
  }
};

void Merge(Set& S, Set& T) {
  if (S.Size() < T.Size()) swap(S, T);
  T.Elements();
  vector<int> elems = T.Elements();
  for (int e : elems) S.Insert(e);
}

void f(int v, vector<Set>& S, vector<int>& grundy,
       const vector<vector<int>>& adj) {
  if (adj[v].empty()) {
    S[v].Insert(0);
    grundy[v] = 1;
    return;
  }

  for (int w : adj[v]) f(w, S, grundy, adj);

  int g = 0;
  for (int w : adj[v]) g ^= grundy[w];
  for (int w : adj[v]) S[w].XOR(g ^ grundy[w]);
  for (int w : adj[v]) Merge(S[v], S[w]);
  if (v != 0) S[v].Insert(g);

  grundy[v] = S[v].Mex();
}

int main() {
  int n;
  cin >> n;

  vector<vector<int>> adj(n + 1);
  for (int i = 0; i < n; i++) {
    int v;
    cin >> v;
    adj[v].push_back(i + 1);
  }
  vector<Set> S(n + 1, Set(n + 2));
  vector<int> grundy(n + 1);

  f(0, S, grundy, adj);

  cout << (grundy[0] ? "Jay" : "ja$on") << endl;
}
