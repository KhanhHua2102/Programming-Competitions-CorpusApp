from decimal import Decimal
from fractions import Fraction

N = int(input())
S = [int(i) for i in input().split()]
C = [Fraction(i) for i in input().split()]
M = [int(i) for i in input().split()]
cache = [Fraction(-1) for _ in range(N)]

def dp(i):
    if i >= N:
        return Fraction(0)
    return cache[i]

for i in range(N-1, -1, -1):
    ans = Fraction(0)
    ans = max(ans, dp(i+1))
    reject = dp(i + M[i] + 1)
    accept = S[i]
    ans = max(ans, reject * (1 - C[i]) + accept * C[i])
    cache[i] = ans

ans = dp(0)
ans = Decimal(ans.numerator) / Decimal(ans.denominator)

print(f"{ans:.18f}")
