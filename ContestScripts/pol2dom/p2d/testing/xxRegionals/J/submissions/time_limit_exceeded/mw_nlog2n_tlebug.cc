#include <bits/stdc++.h>
using namespace std;

vector<int> all;

struct trie {
    int bits;
    struct node {
        int z, o, cnt = 0, x = 0;
    };
    vector<node> nodes;
    trie(int bits) : bits(bits) {
        nodes.push_back({-1, -1});
    }
    void new_nodes(int at) {
        nodes.push_back({-1, -1});
        nodes.push_back({-1, -1});
        nodes[at].z = nodes.size()-2;
        nodes[at].o = nodes.size()-1;
    }
    void push_x(int nid, int d) {
        if (!nodes[nid].x) return;
        if (nodes[nid].x & (1<<(bits-d-1)))
            swap(nodes[nid].z, nodes[nid].o);
        nodes[nodes[nid].z].x ^= nodes[nid].x;
        nodes[nodes[nid].o].x ^= nodes[nid].x;
        nodes[nid].x = 0;
    }
    void x(int x) {
        nodes[0].x ^= x;
    }
    int sz() {
        return nodes[0].cnt;
    }
    void add(int n, int nid = 0, int d = 0) {
        if (d == bits) {
            nodes[nid].cnt = 1;
            return;
        }
        if (nodes[nid].z == -1) new_nodes(nid);
        push_x(nid, d);
        if (n & (1<<(bits-d-1)))
            add(n, nodes[nid].o, d+1);
        else
            add(n, nodes[nid].z, d+1);
        nodes[nid].cnt = nodes[nodes[nid].z].cnt + nodes[nodes[nid].o].cnt;
    }
    void dfs(int nid = 0, int d = 0, int n = 0) {
        if (d == bits) {
            if (nodes[nid].cnt)
                all.push_back(n);
            return;
        }
        push_x(nid, d);
        if (nodes[nid].z != -1) {
            dfs(nodes[nid].z, d+1, n);
            dfs(nodes[nid].o, d+1, n | (1<<(bits-d-1)));
        }
    }
    void fill_all() {
        all.clear();
        dfs();
    }
    int smallest(int n = 0, int nid = 0, int d = 0) {
        if (!nodes[nid].cnt) return n;
        push_x(nid, d);
        if (nodes[nodes[nid].z].cnt < 1<<(bits-d-1))
            return smallest(n, nodes[nid].z, d+1);
        else
            return smallest(n | (1<<(bits-d-1)), nodes[nid].o, d+1);
        return -1;
    }
};

vector<vector<int>> adj;
vector<trie> tr;
vector<int> g;

int f(int at) {
    if (adj[at].empty()) {
        tr[at].add(0);
        return 1;
    }
    int x = 0, mxc = adj[at][0];
    for (int c : adj[at]) {
        g[c] = f(c);
        x ^= g[c];
        if (tr[c].sz() > tr[mxc].sz()) mxc = c;
    }
    for (int c : adj[at])
        tr[c].x(x^g[c]);
    for (int c : adj[at]) {
        if (c != mxc) {
            tr[c].fill_all();
            for (int v : all)
                tr[mxc].add(v);
        }
    }
    swap(tr[at], tr[mxc]);
    if (at != 0) tr[at].add(x);
    tr[at].fill_all();
    int mex = tr[at].smallest();
    return mex;
}

int main() {
    int N;
    cin >> N;
    adj.assign(N+1, vector<int>());
    tr.assign(N+1, trie(20));
    g.assign(N+1, 0);
    for (int i = 1; i <= N; ++i) {
        int p;
        cin >> p;
        adj[p].push_back(i);
    }
    cout << (f(0) ? "Jay" : "ja$on") << endl;
}
