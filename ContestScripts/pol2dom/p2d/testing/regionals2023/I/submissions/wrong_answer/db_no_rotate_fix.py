import math
import sys


def intersect(r1, r2):
    return (max(r1[0], r2[0]), min(r1[1], r2[1]))


def is_empty(r):
    return r[0] > r[1] or r[0] == math.inf or r[1] == -math.inf


def ceil(n, d):
    """Mostly integer ceil... Handles math.inf, too"""
    if n == math.inf or n == -math.inf:
        return n
    return (n + d - 1) // d


def floor(n, d):
    """Mostly integer floor... Handles math.inf, too"""
    if n == math.inf or n == -math.inf:
        return n
    return n // d


def apply(orig, op, value):
    if op == "+":
        return orig + value
    if op == "-":
        return orig - value
    if op == "*":
        if abs(orig) == math.inf:
            return orig
        return orig * value
    if op == "/":
        return floor(orig, value)


class QuasiLinear:
    def __init__(self, ops):
        self.a = 1
        self.b = 0
        self.c = 1
        self._valid_lower = 0

        for op, val in reversed(ops):
            if op == "+":
                self._valid_lower -= val
            if op == "-":
                self._valid_lower += val
            if op == "*":
                if val == 0:
                    self._valid_lower = -math.inf
                self._valid_lower = ceil(self._valid_lower, val)
            if op == "/":
                self._valid_lower *= val
            self._valid_lower = max(0, self._valid_lower)

        for o in ops:
            self._append(o)

    def _append(self, x):
        if x[0] == "+":
            self._add(x[1])
        if x[0] == "-":
            self._add(-x[1])
        if x[0] == "*":
            self._mult(x[1])
        if x[0] == "/":
            self._div(x[1])

    def _add(self, x):
        self.b += x

    def _mult(self, x):
        self.a *= x
        self.b *= x

    def _div(self, x):
        self.c *= x

    def fixed_points(self):
        lower_n = -self.b
        upper_n = self.c - 1 - self.b
        d = self.a - self.c

        if d == 0:
            return (-math.inf, math.inf) if 0 <= self.b < self.c else (0, -1)

        if d < 0:
            lower_n, upper_n, d = -upper_n, -lower_n, -d

        return (ceil(lower_n, d), floor(upper_n, d))

    def non_decreasing_points(self):
        lower_n = -self.b
        upper_n = math.inf
        d = self.a - self.c

        if d == 0:
            return (-math.inf, math.inf) if 0 <= self.b else (0, -1)

        if d < 0:
            lower_n, upper_n, d = -upper_n, -lower_n, -d

        return (ceil(lower_n, d), floor(upper_n, d))

    def valid_points(self):
        return (self._valid_lower, math.inf)


def one_iter(ops, start):
    ans = start
    for o, val in ops:
        ans = apply(ans, o, val)
    return ans


def do_case():
    ops = [(s[0], int(s[1:])) for s in input().split()]
    # print(ops)
    if next((i for i, p in enumerate(ops) if p[0] == "/"), -1) == -1:
        ops.append(("/", 1))
    division_idx = next(i for i, p in enumerate(ops) if p[0] == "/")

    rotated = [ops[(division_idx + i) % len(ops)] for i in range(1, len(ops) + 1)]
    rotated_ql = QuasiLinear(rotated)

    fixed_pts = rotated_ql.fixed_points()
    non_d_pts = rotated_ql.non_decreasing_points()
    print(f"{fixed_pts=} {non_d_pts=}", file=sys.stderr)
    if is_empty(fixed_pts):
        return None

    # TODO: Deal with mult...?
    for op, val in ops[division_idx + 1 :]:
        fixed_pts = (apply(fixed_pts[0], op, val), apply(fixed_pts[1], op, val))
        non_d_pts = (apply(non_d_pts[0], op, val), apply(non_d_pts[1], op, val))
        assert not is_empty(fixed_pts) and not is_empty(non_d_pts)

    print(f"After: {fixed_pts=} {non_d_pts=}", file=sys.stderr)

    orig_ql = QuasiLinear(ops)
    valid_pts = orig_ql.valid_points()
    print(f"{valid_pts=}", file=sys.stderr)
    if is_empty(intersect(valid_pts, fixed_pts)):
        return None

    non_d_pts = intersect(non_d_pts, valid_pts)

    zero = valid_pts[0]
    one = one_iter(ops, zero)
    two = one_iter(ops, one)

    print(two, one, zero, file=sys.stderr)
    if abs(two - non_d_pts[0]) > abs(one - non_d_pts[0]):
        return non_d_pts[0]
    return zero

Q = int(input())
for _ in range(Q):
    ans = do_case()
    if ans is None:
        print("BAD CODE")
    else:
        print(ans)
