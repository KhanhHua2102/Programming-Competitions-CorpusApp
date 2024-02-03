import java.util.*;

public class tbuzzelli_fasterish {
  static final long MOD = 1_000_000_007;
  static int n;
  static long[][] m1, m2, m3;

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
    m1 = new long[n + 1][n + 1];
    m2 = new long[n + 1][n + 1];
    m3 = new long[n + 1][n + 1];
    // print(takeBoss);
    // print(canSkipBoss);
    // print(mult(takeBoss, takeBoss));
    // print(fastMult(takeBoss, takeBoss));
    // print(pow(takeBoss, l));
    // print(pow(canSkipBoss, u - l));
    long[][] mat = fastMult(pow(takeBoss, l), pow(canSkipBoss, u - l));
    System.out.println((mat[0][0] + mat[0][n]) % MOD);
  }

  static void print(long[][] mat) {
    for (int i = 0; i < mat.length; i++) {
      for (long x : mat[i]) {
        System.out.printf("%3d ", x);
      }
      System.out.println();
    }
    System.out.println();
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

  static long[][] fastMult(long[][] a, long[][] b) {
    int n = a.length;
    long[][] c = new long[n][n];
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (i > 0 && i < n - 1 && j < n - 1) {
          c[i][j] = c[i - 1][(j + n - 2) % (n - 1)];
          continue;
        }
        for (int k = 0; k < n; k++) {
          c[i][j] = (c[i][j] + a[i][k] * b[k][j]) % MOD;
        }
      }
    }
    return c;
  }

  static void fasterMult(long[][] a, long[][] b, long[][] c) {
    if (a == c || b == c) System.err.println("ERROR");
    int n = a.length;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (i > 0 && i < n - 1 && j < n - 1) {
          c[i][j] = c[i - 1][(j + n - 2) % (n - 1)];
          continue;
        }
        c[i][j] = 0;
        for (int k = 0; k < n; k++) {
          c[i][j] = (c[i][j] + a[i][k] * b[k][j]) % MOD;
        }
      }
    }
  }

  static long[][] pow(long[][] mat, int x) {
    long[][] tmp = powInternal(mat, x);
    long[][] res = new long[n + 1][n + 1];
    for (int i = 0; i <= n; i++) {
      for (int j = 0; j <= n; j++) {
        res[i][j] = tmp[i][j];
      }
    }
    return res;
  }

  static long[][] powInternal(long[][] mat, int x) {
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
    fasterMult(res, res, m1);
    if ((x % 2) == 1) {
      fasterMult(m1, mat, m2);
    } else {
      long[][] tmp = m1;
      m1 = m2;
      m2 = tmp;
    }
    return m2;
  }
}
