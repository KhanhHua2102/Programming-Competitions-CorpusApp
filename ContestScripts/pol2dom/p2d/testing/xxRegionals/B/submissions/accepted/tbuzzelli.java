import java.util.*;

public class tbuzzelli {
  public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    int n = in.nextInt();
    long min = Long.MAX_VALUE;
    long max = Long.MIN_VALUE;
    long sumNeg = 0, sumPos = 0;
    for (int i = 0; i < n; i++) {
      long x = in.nextLong();
      min = Math.min(min, x);
      max = Math.max(max, x);
      if (x < 0) sumNeg += x;
      else sumPos += x;
    }
    long ans = 0;
    if (max >= 0) {
      if (min < 0) ans = sumPos - sumNeg;
      else ans = sumPos - min * 2;
    } else {
      ans = max * 2 - sumNeg;
    }
    System.out.println(ans);
  }
}
