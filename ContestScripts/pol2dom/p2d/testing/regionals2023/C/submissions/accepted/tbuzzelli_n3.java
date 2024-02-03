import java.util.*;

public class tbuzzelli_n3 {
  static final double oo = 1e50;
  static int n;
  static long[] xs, ys;

  public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    n = in.nextInt();
    xs = new long[n];
    ys = new long[n];
    for (int i = 0; i < n; i++) {
      xs[i] = in.nextLong();
      ys[i] = in.nextLong();
    }
    boolean[][] isValid = new boolean[n][n];
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        isValid[i][j] = isValid[j][i] = isValid(i, j);
      }
    }
    
    for (int start=0; start < n; start++) {
        PriorityQueue<State> pq = new PriorityQueue<>();
        pq.add(new State(start, 0));
        double[] dists = new double[n];
        Arrays.fill(dists, oo);
        dists[start] = 0;
        while (!pq.isEmpty()) {
          State s = pq.poll();
          if (dists[s.v] != s.w) {
            continue;
          }
          for (int next = 0; next < n; next++) {
            double newDist = s.w + dist(s.v, next);
            if (newDist < dists[next] && isValid[s.v][next]) {
              dists[next] = newDist;
              pq.add(new State(next, newDist));
            }
          }
        }
        for (int end=0;end<n;end++)
            System.out.printf("%.9f ", dists[end]);
        System.out.println();
    }
  }

  static double dist(int ai, int bi) {
    return Math.sqrt((xs[ai] - xs[bi]) * (xs[ai] - xs[bi]) + (ys[ai] - ys[bi]) * (ys[ai] - ys[bi]));
  }

  static boolean isValid(int ai, int bi) {
    // Edges of the polygon are valid.
    if (Math.abs(bi - ai) == 1 || (ai == 0 && bi == n - 1) || (bi == 0 && ai == n - 1)) {
      return true;
    }
    long ax = xs[ai], ay = ys[ai];
    long aPrevX = xs[(ai + n - 1) % n] - ax, aPrevY = ys[(ai + n - 1) % n] - ay;
    long aNextX = xs[(ai + 1) % n] - ax, aNextY = ys[(ai + 1) % n] - ay;
    long bx = xs[bi], by = ys[bi];
    long bPrevX = xs[(bi + n - 1) % n] - bx, bPrevY = ys[(bi + n - 1) % n] - by;
    long bNextX = xs[(bi + 1) % n] - bx, bNextY = ys[(bi + 1) % n] - by;
    long abx = bx - ax, aby = by - ay;
    // If the line immediately leaves the polygon, this is invalid.
    if (!inBetween(aNextX, aNextY, aPrevX, aPrevY, abx, aby)
        || !inBetween(bNextX, bNextY, bPrevX, bPrevY, -abx, -aby)) {
      return false;
    }
    for (int ci = 0; ci < n; ci++) {
      // Faster than mod.
      int di = ci == n - 1 ? 0 : ci + 1;
      if (ci == ai || ci == bi || di == ai || di == bi) {
        continue;
      }
      long cx = xs[ci], cy = ys[ci];
      long dx = xs[di], dy = ys[di];
      if (Long.signum(cross(abx, aby, cx - ax, cy - ay))
          == Long.signum(cross(abx, aby, dx - ax, dy - ay))) {
        continue;
      }
      long cdx = dx - cx, cdy = dy - cy;
      if (Long.signum(cross(cdx, cdy, ax - cx, ay - cy))
          == Long.signum(cross(cdx, cdy, bx - cx, by - cy))) {
        continue;
      }
      return false;
    }
    return true;
  }

  // Checks that <cx, cy> is "in between" <ax, ay> and <bx, by>.
  static boolean inBetween(long ax, long ay, long bx, long by, long cx, long cy) {
    if (cross(ax, ay, bx, by) < 0) {
      return !inBetween(bx, by, ax, ay, cx, cy);
    }
    return cross(ax, ay, cx, cy) >= 0 && cross(cx, cy, bx, by) >= 0;
  }

  static long cross(long ax, long ay, long bx, long by) {
    return ax * by - ay * bx;
  }

  static class State implements Comparable<State> {
    int v;
    double w;

    public State(int v, double w) {
      this.v = v;
      this.w = w;
    }

    public int compareTo(State s) {
      return Double.compare(w, s.w);
    }
  }
}
