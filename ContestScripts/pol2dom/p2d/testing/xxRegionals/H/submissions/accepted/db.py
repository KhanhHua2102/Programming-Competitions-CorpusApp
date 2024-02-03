# Try Angles. Darcy Best. Expected: AC

def gcd(a, b):
    if a == 0:
        return (b, 0, 1) if b >= 0 else (-b, 0, -1)
    g, c, d = gcd(b % a, a)
    return g, d - (b // a) * c, c


def FourDist2(P, Q_times_2):
    return (2 * P[0] - Q_times_2[0]) ** 2 + (2 * P[1] - Q_times_2[1]) ** 2


def do_case():
    [x1, y1, x2, y2] = map(int, input().split())
    X, Y = x1 - x2, y1 - y2

    g, y0, x0 = gcd(X, -Y)

    # x = x0 + (X // g) * k
    # y = y0 + (Y // g) * k

    best = (x0, y0)
    for k in range(g // 2 - 1, g // 2 + 2):
        current = (x0 + (X // g) * k, y0 + (Y // g) * k)
        if FourDist2(current, (X, Y)) < FourDist2(best, (X, Y)):
            best = current
    return (best[0] + x2, best[1] + y2)


if __name__ == "__main__":
    T = int(input())
    for _ in range(T):
        ans = do_case()
        print(ans[0], ans[1])
