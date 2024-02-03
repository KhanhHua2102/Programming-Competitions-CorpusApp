#include <bits/stdc++.h>
using namespace std;

struct SCC {
	vector<vector<int>> adj_list;
	vector<int> vis, parent, height;
    vector<unordered_set<int>> seen;
    int curr;
	void dfs(int at) {
		vis[at] = 1;
        seen[curr].insert(at);
		for (int c : adj_list[at])
			if (!vis[c])
				dfs(c);
	}
    int find(int a) {
        return parent[a] == a ? a : parent[a] = find(parent[a]);
    }
    void join(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return;
        if (height[a] < height[b]) swap(a, b);
        parent[b] = a;
        height[a] = max(height[a], height[b] + 1);
    }
	void fill_parents() { // THIS WILL MUTATE adj_list!!!!!
		int n = adj_list.size();
        seen = vector<unordered_set<int>>(n);
        for (int i = 0; i < n; ++i) {
            vis.assign(n, 0);
            curr = i;
            dfs(i);
        }
        parent.resize(n);
        iota(begin(parent), end(parent), 0);
        height.assign(n, 1);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                if (seen[i].count(j) && seen[j].count(i))
                    join(i, j);
        for (int i = 0; i < n; ++i)
            parent[i] = find(i);
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
    int mxmn = -1;
    for (int i = 0; i < N; ++i)
        if (scc.parent[i] == i && leaf[i])
            mxmn = max(mxmn, *min_element(begin(sccs[scc.parent[i]]), end(sccs[scc.parent[i]])));
    for (int i : sccs[scc.parent[mxmn]])
        cout << i+1 << " ";
    cout << endl;
}
