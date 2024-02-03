import math

Q, K = map(int, input().split())
lines = [input() for _ in range(Q)]

ans = []
last = 0
pre, suf = [0] + [-math.inf] * K, [0] + [-math.inf] * K
tot, all = 0, [math.inf] * (K + 1)
for q in lines:
    if q == 'M':
        ans.append(str(last))
        continue

    v = int(q.split()[1])
    tot += v
    all[K] = v
    all.sort()

    if q[0] == 'L':
        for k in range(K, 0, -1):
            pre[k] = max(pre[k - 1], pre[k] + v)
        pre[0] = max(0, pre[0] + v)
    else:
        for k in range(K, 0, -1):
            suf[k] = max(suf[k - 1], suf[k] + v)
        suf[0] = max(0, suf[0] + v)

    sum = 0
    for i in range(K + 1):
        pre[i] = max(pre[i], tot - sum)
        suf[i] = max(suf[i], tot - sum)
        sum += all[i]

    last = max(last, suf[K], pre[K])

print('\n'.join(ans))
