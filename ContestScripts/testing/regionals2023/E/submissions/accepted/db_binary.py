# Uses binary searches and heapq's to find everything.

import heapq


def isqrt_ceil(x):
    lo, hi = 0, x
    while hi - lo >= 10:
        mid = (lo + hi) // 2
        if mid * mid < x:
            lo = mid
        else:
            hi = mid
    while lo * lo < x:
        lo += 1
    return lo


N, S, H = map(int, input().split())
all = []

for x in input().split():
    heapq.heappush(all, int(x))

candidates = []

for hour in range(H):
    while len(all) > 0 and all[0] < S:
        heapq.heappush(candidates, -all[0])
        heapq.heappop(all)

    if len(candidates) > 0:
        S += isqrt_ceil(-candidates[0])
        heapq.heappop(candidates)

print(S)
