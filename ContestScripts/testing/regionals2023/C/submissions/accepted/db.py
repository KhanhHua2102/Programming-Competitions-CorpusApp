import math
import fractions


class Point:
    def __init__(self, xx, yy):
        self.x = xx
        self.y = yy


def distance(p, q):
    return math.hypot(p.x - q.x, p.y - q.y)


def ccw(p, q, r):
    v = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y)
    return 0 if v == 0 else -1 if v < 0 else 1


def in_bounds(p, q, r):
    return min(p.x, r.x) <= q.x <= max(p.x, r.x) and min(p.y, r.y) <= q.y <= max(
        p.y, r.y
    )


def intersect(p1, q1, p2, q2):
    if p1 == p2 or p1 == q2 or q1 == p2 or q1 == q2:
        return False

    o1 = ccw(p1, q1, p2)
    o2 = ccw(p1, q1, q2)
    o3 = ccw(p2, q2, p1)
    o4 = ccw(p2, q2, q1)

    return (
        (o1 != o2 and o3 != o4)
        or (o1 == 0 and in_bounds(p1, p2, q1))
        or (o2 == 0 and in_bounds(p1, q2, q1))
        or (o3 == 0 and in_bounds(p2, p1, q2))
        or (o4 == 0 and in_bounds(p2, q1, q2))
    )


def mid_point_inside(p1, p2, P):
    Q = [Point(2 * p.x, 2 * p.y) for p in P]
    p = Point(p1.x + p2.x, p1.y + p2.y)

    on_boundary = False
    for i, a in enumerate(Q):
        b = Q[(i + 1) % len(Q)]
        if a == p or (ccw(a, b, p) == 0 and in_bounds(a, p, b)):
            on_boundary = True

    c = False
    for i, a in enumerate(Q):
        b = Q[(i + 1) % len(Q)]
        if ((a.y <= p.y < b.y) or (b.y <= p.y < a.y)) and (
            p.x < fractions.Fraction((b.x - a.x) * (p.y - a.y), (b.y - a.y)) + a.x
        ):
            c = not c

    return on_boundary or c


def can_see(p, q, P):
    # Don't exit early in order to be worst case.
    see = True
    for i, p1 in enumerate(P):
        q1 = P[(i + 1) % len(P)]
        if intersect(p, q, p1, q1):
            see = False

    return mid_point_inside(p, q, P) and see


def do_case():
    N = int(input())

    P = [Point(*map(int, input().split())) for _ in range(N)]

    dist = [[math.inf for _ in range(N)] for __ in range(N)]
    for i in range(N):
        for j in range(N):
            if can_see(P[i], P[j], P):
                dist[i][j] = distance(P[i], P[j])

    for k in range(N):
        for i in range(N):
            for j in range(N):
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])

    return dist


ans = do_case()
for row in ans:
    print(' '.join("{0:0.10f}".format(x) for x in row))
