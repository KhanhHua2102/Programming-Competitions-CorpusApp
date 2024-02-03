import java.util.*;

public class tbuzzelli {
  public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    int n = in.nextInt();
    int k = in.nextInt();
    long[] ls = new long[k + 1];
    long[] lp = new long[k + 1];
    long[] la = new long[k + 1];
    long[] rs = new long[k + 1];
    long[] rp = new long[k + 1];
    long[] ra = new long[k + 1];
    long bestM = 0;
    long l = 0;
    long r = 0;
    for (int i = 1; i <= k; i++)
      ls[i] = lp[i] = la[i] = rs[i] = rp[i] = ra[i] = (long) -1e18;
    for (int qi = 0; qi < n; qi++) {
      bestM = Math.max(bestM, Math.max(ls[k], rs[k]));
      for (int i = 0; i <= k; i++) {
        bestM = Math.max(bestM, lp[i] + rp[k - i]);
      }
      char c = in.next().charAt(0);
      if (c == 'M') {
        System.out.println(bestM);
      } else {
        int x = in.nextInt();
        long[] s = ls, p = lp, a = la;
        if (c == 'R') {
          s = rs;
          p = rp;
          a = ra;
        }
        for (int i = k; i >= 1; i--)
          s[i] = Math.max(s[i] + x, s[i - 1]);
        s[0] = Math.max(s[0] + x, 0);
        for (int i = k; i >= 1; i--)
          a[i] = Math.max(a[i] + x, a[i - 1]);
        a[0] += x;
        for (int i = 0; i <= k; i++)
          p[i] = Math.max(p[i], a[i]);
      }
    }
  }
}
