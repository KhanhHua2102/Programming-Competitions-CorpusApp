#include <bits/stdc++.h>

#include "bounds.h"
#include "testlib.h"

using namespace std;


pair<bool, vector<int>> ReadAndCheckAnswer(InStream& in, const vector<int>& source, const vector<unordered_set<int>>& froms, const vector<int>& to) {
    int m = to.size(), n = source.size();
    int len = in.readInt(-1, m, "len");
    if (len == -1) return {false, {}};
    vector<int> path(len);
    for (int i = 0; i < len; ++i) {
        path[i] = in.readInt(1, m, "path_i");
        --path[i];
    }

    vector<int> seen = source, used(n), is_to(n);

    for (int i : path) {
        for (int j : froms[i]) {
            used[j] = 1;
            if (!seen[j]) in.quitf(_wa, "Dependent node %d not seen before edge %d", j+1, i+1);
        }
        if (seen[to[i]]) in.quitf(_wa, "Path visits node %d twice", to[i]+1);
        seen[to[i]] = 1;
        is_to[to[i]] = 1;
    }
 
    for (int i = 0; i < n-1; ++i) {
        if (is_to[i] && !used[i]) in.quitf(_wa, "Node %d is a sink but isn't the last node. Path is not minimal", i+1);
    }

    if (!seen[n-1]) in.quitf(_wa, "Path does not visit last node");

    return {true, path};
}

int main(int argc, char* argv[]) {
    registerTestlibCmd(argc, argv);

    int n = inf.readInt(MIN_N, MAX_N, "n"), m = inf.readInt(MIN_M, MAX_M, "m");
    vector<unordered_set<int>> froms(m);
    vector<int> to(m), source(n, 1);

    for (int i = 0; i < m; i++) {
        to[i] = inf.readInt(1, n, "to") - 1;
        int k = inf.readInt(MIN_K, MAX_K, "k");
        source[to[i]] = 0;
        for (int j = 0; j < k; j++) {
            int from = inf.readInt(1, n, "from") - 1;
            froms[i].insert(from);
        }
    }

    auto judge = ReadAndCheckAnswer(ans, source, froms, to);
    auto contestant = ReadAndCheckAnswer(ouf, source, froms, to);

    if (!judge.first && contestant.first)
        quitf(_fail, "ERROR! Contestant found answer, but judge did not. ERROR!");

    if (judge.first && !contestant.first)
        quitf(_wa, "Judge found a solution, but the contestant did not.");

    string judge_str = "";
    for (int i : judge.second) judge_str += to_string(i) + " ";
    string contestant_str = "";
    for (int i : contestant.second) contestant_str += to_string(i) + " ";

    quitf(_ok, "ok, size of judge solution = %d, size of contestant solution = %d; judge solution {%s}, contestant solution {%s}", judge.second.size(), contestant.second.size(), judge_str.c_str(), contestant_str.c_str());
}
