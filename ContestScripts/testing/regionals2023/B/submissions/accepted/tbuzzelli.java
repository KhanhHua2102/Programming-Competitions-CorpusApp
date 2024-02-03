import java.util.*;

public class tbuzzelli {
  public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    int n = in.nextInt();
    int k = n * n - in.nextInt();
    if (k == 1 && n == 1) {
      System.out.println("POSSIBLE");
      System.out.println(".");
      return;
    }
    k -= n;
    if (k < 0 || k > n - 2) {
      System.out.println("IMPOSSIBLE");
      return;
    }
    char[][] grid = new char[n][n];
    for (char[] a : grid) {
      Arrays.fill(a, '#');
    }
    for (int i = 0; i < n - 1; i++) {
      if (i == 0 || k-- > 0) {
        grid[n - 1][i] = grid[i][n - 1] = '.';
      } else {
        grid[i][i] = '.';
      }
    }
    System.out.println("POSSIBLE");
    for (char[] a : grid) {
      System.out.println(new String(a));
    }
  }
}
