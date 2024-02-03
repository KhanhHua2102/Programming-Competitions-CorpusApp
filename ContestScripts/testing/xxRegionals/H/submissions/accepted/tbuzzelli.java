import java.util.*;

public class tbuzzelli {
  public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    int n = in.nextInt();
    for (int i = 0; i < n; i++) {
      long x0 = in.nextLong(), y0 = in.nextLong();
      long x1 = in.nextLong(), y1 = in.nextLong();
      long dx = x1 - x0;
      long dy = y1 - y0;
      long xSign = dx >= 0 ? 1 : -1;
      long ySign = dy >= 0 ? 1 : -1;
      long[] ans = solve(dx * xSign, dy * ySign);
      System.out.printf("%d %d\n", x0 + ans[0] * xSign, y0 + ans[1] * ySign);
    }
  }

  static long[] solve(long x, long y) {
    if (x > y) {
      long[] tmp = solve(y, x);
      return new long[] {tmp[1], tmp[0]};
    }
    if (x == 0) return new long[] {1, y / 2};
    long gcd = gcd(x, y);
    if (gcd != 1) {
      long[] ans = solve(x /= gcd, y /= gcd);
      long scale = gcd / 2;
      ans[0] += x * scale;
      ans[1] += y * scale;
      return ans;
    }
    long mi = modInverse(x, y);
    long dx = mi * x / y, dy = mi;
    if (dy > y / 2) {
      dy = y - dy;
      dx = (dy * x + y - 1) / y;
    }
    if (mi == 0) dy++;
    return new long[] {dx, dy};
  }

  static long gcd(long a, long b) {
    if (b == 0) return a;
    return gcd(b, a % b);
  }

  static long modInverse(long a, long b) {
    return ((eEuclid(a, b)[1] % b) + b) % b;
  }

  static long[] eEuclid(long a, long b) {
    if (b == 0) return new long[] { a, 1, 0 };
    long[] ans = eEuclid(b, a % b);
    long temp = ans[1] - ans[2] * (a / b);
    ans[1] = ans[2];  ans[2] = temp;
    return ans;
  }
}
