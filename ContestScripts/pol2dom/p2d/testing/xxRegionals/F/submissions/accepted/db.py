# SCC code taken from:
# https://www.programiz.com/dsa/strongly-connected-components

import sys
import threading
threading.stack_size(2**27-2**12)
sys.setrecursionlimit(2 * 10**6)
from collections import defaultdict


class Graph:
    def __init__(self, vertex):
        self.V = vertex
        self.graph = defaultdict(list)
        self.current = []

    def add_edge(self, s, d):
        self.graph[s].append(d)

    def dfs(self, d, visited_vertex):
        visited_vertex[d] = True
        self.current.append(d)
        for i in self.graph[d]:
            if not visited_vertex[i]:
                self.dfs(i, visited_vertex)

    def fill_order(self, d, visited_vertex, stack):
        visited_vertex[d] = True
        for i in self.graph[d]:
            if not visited_vertex[i]:
                self.fill_order(i, visited_vertex, stack)
        stack = stack.append(d)

    def transpose(self):
        g = Graph(self.V)

        for i in self.graph:
            for j in self.graph[i]:
                g.add_edge(j, i)
        return g

    def get_scc(self):
        stack = []
        visited_vertex = [False] * (self.V)

        for i in range(self.V):
            if not visited_vertex[i]:
                self.fill_order(i, visited_vertex, stack)

        gr = self.transpose()

        visited_vertex = [False] * (self.V)

        component = [-1] * self.V
        while stack:
            i = stack.pop()
            if not visited_vertex[i]:
                gr.current = []
                gr.dfs(i, visited_vertex)
                m = min(gr.current)
                for v in gr.current:
                    component[v] = m
        return component


n = int(input())
G = Graph(n)
edges = []
for i in range(n):
    out = list(map(int, input().split()))[1:]
    for v in out:
        G.add_edge(i, v - 1)
        edges.append((i, v - 1))

comp = G.get_scc()

leaf = [False] * n
for c in comp:
    leaf[c] = True
for u, v in edges:
    if comp[u] != comp[v]:
        leaf[comp[u]] = False

winner_comp = max(c for c in comp if leaf[c])
winners = list(filter(lambda x: comp[x - 1] == winner_comp, range(1, n + 1)))
print(' '.join(map(str, winners)))
