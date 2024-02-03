# We do not need to consider this for time limits; it is just checking precision.

import decimal

decimal.getcontext().prec = 100

N = int(input())
S = list(map(decimal.Decimal, input().split()))
C = list(map(decimal.Decimal, input().split()))
M = list(map(int, input().split()))

ans = [decimal.Decimal(0) for _ in range(2 * N + 3)]
for i in range(N - 1, -1, -1):
    ans[i] = max(
        ans[i + 1], C[i] * S[i] + (decimal.Decimal(1) - C[i]) * (ans[i + 1 + M[i]])
    )

print("{0:0.12f}".format(float(ans[0])))
