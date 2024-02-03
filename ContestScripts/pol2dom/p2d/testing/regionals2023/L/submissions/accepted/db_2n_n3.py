# O(2^N * N^3)


def g(setA, setB, current_idx, N, A):
    if current_idx == N + 1:
        return 0

    ans = 0

    # Put current_idx into setA
    amtA = 0
    for b in setB:
        for f in A:
            if f == (b, current_idx):
                amtA += 1
    setA.append(current_idx)
    ans = max(ans, amtA + g(setA, setB, current_idx + 1, N, A))
    setA.pop(-1)

    # Put current_idx into setB
    amtB = 0
    for a in setA:
        for f in A:
            if f == (a, current_idx):
                amtB += 1
    setB.append(current_idx)
    ans = max(ans, amtB + g(setA, setB, current_idx + 1, N, A))
    setB.pop(-1)

    return ans


N, F = map(int, input().split())
A = [tuple(map(int, input().split())) for _ in range(F)]

setA, setB = [], []
print(g(setA, setB, 1, N, A))
