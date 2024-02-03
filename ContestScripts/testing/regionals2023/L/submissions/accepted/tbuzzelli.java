import java.util.*;

public class tbuzzelli {
  public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    int n = in.nextInt();
    int m = in.nextInt();
    int[] as = new int[m];
    int[] bs = new int[m];
    for (int i = 0; i < m; i++) {
      as[i] = 1 << (in.nextInt() - 1);
      bs[i] = 1 << (in.nextInt() - 1);
    }
    int max = 0;
    for (int mask = 0; mask < (1 << n); mask++) {
      int cnt = 0;
      for (int i = 0; i < m; i++) {
        if (((mask & as[i]) != 0) != ((mask & bs[i]) != 0)) {
          cnt++;
        }
      }
      if (cnt > max) {
        max = cnt;
      }
    }
    System.out.println(max);
  }
}
