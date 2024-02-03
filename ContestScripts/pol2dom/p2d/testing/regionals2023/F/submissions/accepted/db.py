import sys

# Change recursion limits
import threading

threading.stack_size(2**27 - 2**12)
sys.setrecursionlimit(10**5)

N = int(input())
P = [tuple(map(int, input().split())) for _ in range(N)]
DP = dict()


def same_direction(orig, p, q):
    if p == q:
        return False
    a = (p[0] - orig[0], p[1] - orig[1])
    b = (q[0] - p[0], q[1] - p[1])
    return a[0] * b[0] + a[1] * b[1] >= 0


def f(v):
    if v in DP:
        return DP[v]

    ans = 1
    for u in P:
        if same_direction(P[-1], u, v):
            ans = max(ans, f(u) + 1)

    DP[v] = ans
    return ans


print(f(P[0]) - 2)
