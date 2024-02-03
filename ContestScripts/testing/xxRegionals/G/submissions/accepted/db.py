import math
import heapq

MOD = 10 ** 9 + 7

n, m = map(int, input().split())
adj = [[] for _ in range(n)]
back = [[] for _ in range(n)]
edges = []

for _ in range(m):
    u, v, w = map(int, input().split())
    adj[u - 1].append((v - 1, w))
    back[v - 1].append((u - 1, w))
    edges.append((u - 1, v - 1, w))


def dijkstra(adj, src):
    n = len(adj)
    dist = [-math.inf] * n

    pq = []
    heapq.heappush(pq, (0, src))

    while pq:
        d, v = heapq.heappop(pq)
        if dist[v] != -math.inf:
            continue
        dist[v] = d

        for u, w in adj[v]:
            heapq.heappush(pq, (d + w, u))

    return dist


def count_shortest_paths(adj, src):
    dist = dijkstra(adj, src)
    n = len(adj)
    order = sorted([(dist[v], v) for v in range(n)])
    paths = [-math.inf] * n
    for d, v in order:
        if v == src:
            paths[v] = 1

        for u, w in adj[v]:
            if dist[u] == dist[v] + w:
                if paths[u] == -math.inf:
                    paths[u] = paths[v]
                else:
                    paths[u] = (paths[u] + paths[v]) % MOD
    return paths, dist


pathA, distA = count_shortest_paths(adj, 0)
pathB, distB = count_shortest_paths(back, n - 1)

total = 0 if pathA[n - 1] == -math.inf else pathA[n - 1]
ans = []

for u, v, w in edges:
    if -math.inf in {distA[u], distA[v], distB[u], distB[v]}:
        ans.append(total)
    elif distA[u] + w + distB[v] == distB[0]:
        ans.append((total - pathA[u] * pathB[v]) % MOD)
    else:
        ans.append(total)

print('\n'.join(map(str, ans)))
