k = int(input())
S = input()
J = input()


def Z(s):
    n = len(s)
    z = [0] * n
    L, R = 0, 0
    for i in range(1, n):
        if i <= R:
            z[i] = min(R - i + 1, z[i - L])
        while i + z[i] < n and s[z[i]] == s[i + z[i]]:
            z[i] += 1
        if i + z[i] - 1 > R:
            L, R = i, i + z[i] - 1
    return z


T = J + '@' + ('#'.join(S[::i + 1] for i in range(k + 1)) + '#')
z = Z(T)

count = 0
ans = []
for i in range(len(J) + 1, len(T)):
    if T[i] == '#':
        ans.append(str(count))
        count = 0
    if z[i] == len(J):
        count += 1

print('\n'.join(ans))
