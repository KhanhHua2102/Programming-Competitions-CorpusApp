N = int(input())
W = []
C = []
D = []
for i in range(N):
    w, c, d = map(int, input().split())
    W.append(w)
    C.append(c)
    D.append(d)

def dfs(curweight, remaining):
    if remaining == 0: return 0
    # don't allow stopping early.
    best = -1000000000
    for i in range(N):
        if not (remaining & (1 << i)):
            continue

        if curweight + W[i] <= C[i]:
            best = max(best, D[i] + dfs(curweight + W[i], remaining ^ (1 << i)))
    return best

print(dfs(0, (1 << N) - 1))
