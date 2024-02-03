#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct RangeTree {
	int N; vector<T> node_vals;
	T base_value;
	int ceil_pow_2(int v) { // Nearest power of two upper bound.
		int p; for (p = 1; p < v; p *= 2) {} return p;
	}
	RangeTree(int sz, T base) : base_value(base), N(ceil_pow_2(sz)) {
		node_vals.resize(2*N-1, base_value);
	}
	T combine(T a, T b) { return a.first < b.first ? a : b; } // Could be min or +.
	T query(int l, int r, int nid, int nl, int nr) {
		if (l == nl && r == nr)
			return node_vals[nid];
		int mid = (nl+nr)/2; T result = base_value;
		if (l <= mid)
			result = combine(result, query(l, min(r, mid), 2*nid+1, nl, mid));
		if (r > mid)
			result = combine(result, query(max(l, mid+1), r, 2*nid+2, mid+1, nr));
		return result;
	}
	T query(int l, int r) { return query(l, r, 0, 0, N-1); }
	void update(int index, T value, int nid, int nl, int nr) {
		if (nl == nr) { node_vals[nid] = value; return; }
		int mid = (nl+nr)/2;
		if (index <= mid) 	update(index, value, 2*nid+1, nl, mid);
		else				update(index, value, 2*nid+2, mid+1, nr);
		node_vals[nid] = combine(node_vals[2*nid+1], node_vals[2*nid+2]);
	}
	void update(int index, T value) { update(index, value, 0, 0, N-1); }
	// Optional
	void print() {
		for (int i : node_vals)
			cout << i << " ";
		cout << endl;
	}
};

int main() {
    int N;
    cin >> N;
    vector<int> L(N), R(N);
    vector<int> locs;
    for (int i = 0; i < N; ++i) {
        cin >> L[i] >> R[i];
        locs.push_back(L[i]);
        locs.push_back(R[i]);
    }
    sort(locs.begin(), locs.end());
    locs.erase(unique(locs.begin(), locs.end()), locs.end());
    map<int, set<pair<int, int>>> LRs;
    int curr = 0;
    for (int i = 0; i < N; ++i) {
        L[i] = lower_bound(locs.begin(), locs.end(), L[i]) - locs.begin();
        R[i] = lower_bound(locs.begin(), locs.end(), R[i]) - locs.begin();
        LRs[L[i]].emplace(R[i], i);
        if (R[i] < R[curr]) curr = i;
    }
    RangeTree<pair<int, int>> rt(locs.size(), make_pair(1e9, -1));
    for (auto& p : LRs)
        rt.update(p.first, *p.second.begin());
    vector<int> ans{curr};
    for (int i = 0; i < N-1; ++i) {
        LRs[L[curr]].erase(LRs[L[curr]].begin());
        if (LRs[L[curr]].size())
            rt.update(L[curr], *LRs[L[curr]].begin());
        else
            rt.update(L[curr], make_pair(1e9, -1));
        curr = rt.query(0, R[curr]).second;
        if (curr == -1) {
            cout << "NO" << endl;
            return 0;
        }
        ans.push_back(curr);
    }
    cout << "YES" << endl;
    for (auto& x : ans) cout << x+1 << " ";
    cout << endl;
}
