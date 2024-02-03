MOD = 10**9 + 7


def mult(A, B):
    n = len(B)
    C = [B[0]] + B[n - 1 : 0 : -1]
    R = []

    for i in range(n):
        R.append(sum(A[j] * C[(j - i) % n] for j in range(n)) % MOD)

    return R


def f(X, m):
    if m == 0:
        return [1] + [0] * (len(X) - 1)

    if m % 2 == 0:
        Y = mult(X, f(X, m - 1))
        Y[0] += 1
        return Y

    XI = X.copy()
    XI[0] += 1

    return mult(XI, f(mult(X, X), m // 2))


n, k, L, U = map(int, input().split())
B = map(int, input().split())

A = [0] * k
for b in B:
    A[b % k] += 1

pows = [[1] + [0] * (len(A) - 1)]
for i in range(1, 5):
    pows.append(mult(pows[-1], A))

print((f(A, U)[0] - f(A, L - 1)[0]) % MOD)
