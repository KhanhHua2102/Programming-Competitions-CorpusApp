def construct(n, k):
    d = n**2 - k
    if n == 1 and k == 0:
        return [["."]]
    if not (n <= d <= 2 * n - 2):
        return None

    A = [["#" for _ in range(n)] for __ in range(n)]

    mid = 2 * n - 2 - d
    for i in range(mid):
        A[i][i] = "."

    for i in range(mid + 1, n):
        A[mid][i] = "."
        A[i][mid] = "."

    return A


n, k = map(int, input().split())
A = construct(n, k)

print("IMPOSSIBLE" if A is None else "POSSIBLE")
if A is not None:
    for row in A:
        print("".join(row))
