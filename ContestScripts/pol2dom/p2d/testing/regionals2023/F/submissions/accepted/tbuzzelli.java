import java.util.*;

public class tbuzzelli {
  static Vec[] ps;
  static int n;
  static int[] memo;

  public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    n = in.nextInt();
    ps = new Vec[n];
    for (int i = 0; i < n; i++) {
      ps[i] = new Vec(in.nextLong(), in.nextLong());
    }
    memo = new int[n];
    Arrays.fill(memo, -1);
    System.out.println(go(0));
  }

  static int go(int i) {
    if (memo[i] != -1) {
      return memo[i];
    }
    int ans = 0;
    for (int j = 0; j < n - 1; j++) {
      if (i != j && isValid(i, j)) {
        ans = Math.max(ans, go(j) + 1);
      }
    }
    return memo[i] = ans;
  }

  static boolean isValid(int i, int j) {
    Vec a = ps[i], b = ps[j], c = ps[n - 1];
    Vec ab = b.sub(a);
    return ab.dot(c.sub(a)) >= ab.dot(ab);
  }

  static class Vec {
    long x, y;

    Vec(long xx, long yy) {
      x = xx;
      y = yy;
    }

    Vec sub(Vec v) {
      return new Vec(x - v.x, y - v.y);
    }

    long dot(Vec v) {
      return x * v.x + y * v.y;
    }
  }
}
