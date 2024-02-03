n = int(input())
layers = [tuple(map(int, input().split())) for _ in range(n)]


def f(used, layers, weight):
    best = 0

    for i in range(n):
        if not used[i]:
            w, c, d = layers[i]
            if w + weight <= c:
                used[i] = True
                best = max(best, d + f(used, layers, weight + w))
                used[i] = False

    return best


used = [False] * n
print(f(used, layers, 0))
