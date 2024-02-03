import java.util.*;

public class tbuzzelli_k3_wa {
  static final long MOD = 1_000_000_007;
  static int n;

  public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    int m = in.nextInt();
    n = in.nextInt();
    int l = in.nextInt();
    int u = in.nextInt();
    List<Integer> bosses = new ArrayList<>();
    for (int i = 0; i < m; i++) {
      bosses.add(in.nextInt());
    }
    long[][] takeBoss = buildTransitionMatrix(bosses, false);
    long[][] canSkipBoss = buildTransitionMatrix(bosses, true);
    long[][] mat = mult(pow(takeBoss, l), pow(canSkipBoss, u - l));
    System.out.println(mat[0][0] + mat[0][n]);
  }

  static long[][] buildTransitionMatrix(List<Integer> bosses, boolean canSkip) {
    long[][] mat = new long[n + 1][n + 1];
    for (int boss : bosses) {
      for (int i = 0; i < n; i++) {
        mat[i][(i + boss) % n]++;
      }
    }
    if (canSkip) {
      mat[0][n] = mat[n][n] = 1;
    }
    return mat;
  }

  static long[][] mult(long[][] a, long[][] b) {
    int n = a.length;
    long[][] c = new long[n][n];
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
          c[i][j] = (c[i][j] + a[i][k] * b[k][j]) % MOD;
        }
      }
    }
    return c;
  }

  static long[][] pow(long[][] mat, int x) {
    int n = mat.length;
    if (x == 0) {
      long[][] res = new long[n][n];
      for (int i = 0; i < n; i++) {
        res[i][i] = 1;
      }
      return res;
    }
    if (x == 1) {
      return mat;
    }
    long[][] res = pow(mat, x / 2);
    res = mult(res, res);
    if ((x % 2) == 1) {
      res = mult(res, mat);
    }
    return res;
  }
}
