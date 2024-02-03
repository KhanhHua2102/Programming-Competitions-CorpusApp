import java.util.*;

public class tbuzzelli {
  static int n;
  static int[] ws, cs, ds;

  public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    n = in.nextInt();
    ws = new int[n];
    cs = new int[n];
    ds = new int[n];
    for (int i = 0; i < n; i++) {
      ws[i] = in.nextInt();
      cs[i] = in.nextInt();
      ds[i] = in.nextInt();
    }
    System.out.println(go(0, 0, 0, 0));
  }

  static long go(int i, int w, long d, int mask) {
    long res = d;
    for (int j = 0; j < n; j++) {
      int bit = 1 << j;
      if ((mask & bit) != 0 || w + ws[j] > cs[j]) continue;
      res = Math.max(res, go(i + 1, w + ws[j], d + ds[j], mask | bit));
    }
    return res;
  }
}
