N = int(input())
S = list(map(int, input().split()))
C = list(map(float, input().split()))
M = list(map(int, input().split()))

ans = [0.0 for _ in range(2 * N + 3)]
for i in range(N - 1, -1, -1):
    ans[i] = max(ans[i + 1], C[i] * S[i] + (1 - C[i]) * (ans[i + 1 + M[i]]))

print("{0:0.10f}".format(ans[0]))
