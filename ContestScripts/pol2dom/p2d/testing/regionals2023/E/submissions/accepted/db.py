import math


def isqrt_ceil(x):
    sqrt_x = math.isqrt(x)
    if sqrt_x * sqrt_x < x:
        sqrt_x += 1
    return sqrt_x


N, S, H = map(int, input().split())
A = list(reversed(sorted(map(int, input().split()))))
B = []

for hour in range(H):
    while len(A) > 0 and A[-1] < S:
        B.append(A[-1])
        A.pop(-1)

    if len(B) > 0:
        S += isqrt_ceil(B[-1])
        B.pop(-1)

print(S)
