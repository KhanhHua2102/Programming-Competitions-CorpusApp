// Compares clusters directly by looking at their nodes every time. This is O(n^2) potentially.
#include <bits/stdc++.h>
using namespace std;

struct SCC {
	stack<int> stk;
	vector<vector<int>> adj_list;
	vector<int> parent;
	bool fill_stk;
	void dfs(int at, int colour) {
		parent[at] = colour;
		for (int child : adj_list[at])
			if (parent[child] == -1)
				dfs(child, colour);
		if (fill_stk) stk.push(at);
	}
	void fill_parents() { // THIS WILL MUTATE adj_list!!!!!
		const int N = adj_list.size();
		parent = vector<int>(N, -1);
		fill_stk = true;
		for (int i = 0; i < N; ++i) // Get dfs order on stack.
			if (parent[i] == -1) dfs(i, i);
		vector<vector<int>> transposed(N);
		for (int i = 0; i < N; ++i) // Transpose graph.
			for (int j : adj_list[i])
				transposed[j].push_back(i);
		adj_list = transposed;
		parent = vector<int>(N, -1);
		fill_stk = false;
		while (!stk.empty()) { // Find SCCs.
			int i = stk.top(); stk.pop();
			if (parent[i] == -1)
				dfs(i, i);
		}

	}
};

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
    int N, M;
    cin >> N;
    SCC scc;
    scc.adj_list = vector<vector<int>>(N);
    for (int i = 0; i < N; ++i) {
        cin >> M;
        for (int j = 0; j < M; ++j) {
            int x;
            cin >> x;
            scc.adj_list[i].push_back(x-1);
        }
    }
    auto alcpy = scc.adj_list;
    scc.fill_parents();
    vector<vector<int>> sccs(N);
    for (int i = 0; i < N; ++i)
        sccs[scc.parent[i]].push_back(i);
    vector<int> leaf(N, 1);
    for (int i = 0; i < N; ++i)
        for (int j : alcpy[i])
            if (scc.parent[i] != scc.parent[j])
                leaf[scc.parent[i]] = 0;
    int c = -1;
	unordered_set<int> cs;
    for (int i = 0; i < N; ++i) {
        if (scc.parent[i] == i && leaf[i]) {
			unordered_set<int> is(sccs[i].begin(), sccs[i].end());
			if (c == -1) {
				c = i;
				cs = is;
				continue;
			}
			int mn = 1e9;
			for (int j : sccs[c])
				if (!is.count(j))
					mn = min(mn, j);
			for (int j : sccs[i])
				if (!cs.count(j))
					mn = min(mn, j);
			if (mn < 1e9 && scc.parent[mn] == c) {
				c = i;
				cs = is;
			}
		}
	}
    for (int i : sccs[scc.parent[c]])
        cout << i+1 << " ";
    cout << endl;
}
