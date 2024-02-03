MOD = 10**9 + 7

N, alpha = map(int, input().split())
SS = input()
A = [list(map(int, input().split())) for _ in range(alpha)]

S = [ord(x) - ord("A") for x in SS]
inside_DP = [[None for _ in range(N + 2)] for __ in range(N + 2)]
outside_DP = [[None for _ in range(N + 2)] for __ in range(N + 2)]


def inside(L, R):
    if inside_DP[L + 1][R + 1] is not None:
        return inside_DP[L + 1][R + 1]
    if L + 1 == R:
        return 1

    ans = 0
    for r in range(L + 2, R):
        me = A[S[L + 1]][S[r]]
        left = inside(L + 1, r)
        right = inside(r, R)
        ans += me * left * right

    inside_DP[L + 1][R + 1] = ans % MOD
    return ans


def outside(L, R):
    if outside_DP[L + 1][R + 1] is not None:
        return outside_DP[L + 1][R + 1]
    if L <= 0:
        outside_DP[L + 1][R + 1] = inside(R, N)
        return outside_DP[L + 1][R + 1]

    ans = 0
    for l in range(L - 1):
        me = A[S[l]][S[L - 1]]
        under = inside(l, L - 1)
        over = outside(l, R)
        ans += me * under * over

    for r in range(R + 1, N):
        me = A[S[L - 1]][S[r]]
        under = inside(R, r)
        over = outside(L - 1, r)
        ans += me * under * over

    outside_DP[L + 1][R + 1] = ans % MOD
    return ans


for idx in range(N):
    for ch in range(alpha):
        ans = 0
        for l in range(idx):
            ans += A[S[l]][ch] * inside(l, idx) * outside(l, idx)
        for r in range(idx + 1, N):
            ans += A[ch][S[r]] * inside(idx, r) * outside(idx, r)
        print(ans % MOD)
