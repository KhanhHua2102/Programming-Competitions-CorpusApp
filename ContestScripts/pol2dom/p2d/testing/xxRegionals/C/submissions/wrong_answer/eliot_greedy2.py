N = int(input())
V = []
for i in range(N):
    w, c, d = map(int, input().split())
    V.append((w, c, d))
# sort by excess carrying capacity, then carrying capacity, then the deliciousness to weight ratio, then by deliciousness
V.sort(key=lambda x: (x[1] - x[0], x[1], x[2]/x[0], x[2]), reverse=True)
W = []
C = []
D = []
for i in range(N):
    w, c, d = V[i]
    W.append(w)
    C.append(c)
    D.append(d)

best = 0
excess = 1000000000000
for (w, c, d) in V:
    if excess - w >= 0 and c - w >= 0:
        excess = min(excess - w, c - w)
        best += d
print(best)
