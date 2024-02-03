import java.util.*;

public class tbuzzelli_slower_mod {

  static final long MOD = 1_000_000_007L;
  static int n;
  static long[][] memoIn;
  static long[][] memoOut;
  static char[] str;
  static long[][] map;

  public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    n = in.nextInt();
    int a = in.nextInt();
    map = new long[256][256];
    str = in.next().toCharArray();
    for (int i = 0; i < a; i++) {
      for (int j = 0; j < a; j++) {
        map['A' + i]['A' + j] = in.nextLong();
      }
    }
    memoIn = new long[n][n];
    for (long[] arr : memoIn) {
      Arrays.fill(arr, -1L);
    }
    memoOut = new long[n][n];
    for (long[] arr : memoOut) {
      Arrays.fill(arr, -1L);
    }
    for (int i = 0; i < n; i++) {
      for (char ch = 'A'; ch < 'A' + a; ch++) {
        long ans = 0;
        for (int k = i - 1; k >= 0; k -= 1) {
          long tmp = (goOut(k, i) * goIn(k + 1, i - 1)) % MOD;
          tmp = (tmp * map[str[k]][ch]) % MOD;
          ans += tmp;
          ans %= MOD;
        }
        for (int k = i + 1; k < n; k += 1) {
          long tmp = (goOut(i, k) * goIn(i + 1, k - 1)) % MOD;
          tmp = (tmp * map[ch][str[k]]) % MOD;
          ans += tmp;
          ans %= MOD;
        }
        ans %= MOD;
        System.out.println(ans);
      }
    }
  }

  static long goIn(int i, int j) {
    if (i > j) {
      return 1;
    }
    if (memoIn[i][j] != -1) {
      return memoIn[i][j];
    }
    long ans = 0;
    for (int k = i + 1; k <= j; k += 1) {
      long tmp = (goIn(i + 1, k - 1) * goIn(k + 1, j)) % MOD;
      tmp = (tmp * map[str[i]][str[k]]) % MOD;
      ans += tmp;
      ans %= MOD;
    }
    return memoIn[i][j] = ans;
  }

  static long goOut(int i, int j) {
    if (i == 0 && j == n - 1) {
      return 1;
    }
    if (memoOut[i][j] != -1) {
      return memoOut[i][j];
    }
    long ans = 0;
    if (i == 0) {
      ans = goIn(j + 1, n - 1);
    } else if (j == n - 1) {
      ans = goIn(0, i - 1);
    } else {
      for (int k = i - 2; k >= 0; k -= 1) {
        long tmp = (goOut(k, j) * goIn(k + 1, i - 2)) % MOD;
        tmp = (tmp * map[str[k]][str[i - 1]]) % MOD;
        ans += tmp;
        ans %= MOD;
      }
      for (int k = j + 1; k < n; k += 1) {
        long tmp = (goOut(i - 1, k) * goIn(j + 1, k - 1)) % MOD;
        tmp = (tmp * map[str[i - 1]][str[k]]) % MOD;
        ans += tmp;
        ans %= MOD;
      }
    }
    return memoOut[i][j] = ans;
  }
}
