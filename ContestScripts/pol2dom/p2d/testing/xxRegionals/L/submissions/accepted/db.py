import heapq
import math


n, L = map(int, input().split())
land = [tuple(map(int, input().split())) for _ in range(n)]

suffixS = [0] * (n + 1)
for i in range(n - 1, -1, -1):
    suffixS[i] = suffixS[i + 1] + land[i][1]


def mark_deleted(options, val):
    options[1].add(val[3])


def is_deleted(options, val):
    return val[3] in options[1]


def pop(maximums, options):
    opt = maximums.pop(-1)
    mark_deleted(options, opt)
    return opt


def add(maximums, options, a, b, start_idx, last_idx):
    val = (a + b, a, b, start_idx, last_idx)
    maximums.append(val)
    heapq.heappush(options[0], val)


def extract(options, reach):
    while options[0][0][3] > reach or is_deleted(options, options[0][0]):
        val = heapq.heappop(options[0])

    if options[0][0][4] > reach:
        val = heapq.heappop(options[0])
        new_val = (val[1] + DP[reach + 1], val[1],
                   DP[reach + 1], val[3], reach)
        heapq.heappush(options[0], new_val)
    while options[0][0][3] > reach or is_deleted(options, options[0][0]):
        heapq.heappop(options[0])
    return options[0][0]


DP = [0] * (n + 1)
reach = n - 1  # inclusive reach point
maximums = []
options = (list(), set())
add(maximums, options, math.inf, 0, -1, -1)
for i in range(n - 1, -1, -1):
    V = land[i][0]
    while suffixS[i] - suffixS[reach + 1] > L:
        reach -= 1

    last_dp, last_idx = DP[i + 1], i
    while maximums[-1][1] <= V:
        _, _, last_dp, start_idx, last_idx = pop(maximums, options)
    add(maximums, options, V, last_dp, i, last_idx)

    DP[i] = extract(options, reach)[0]

print(DP[0])
