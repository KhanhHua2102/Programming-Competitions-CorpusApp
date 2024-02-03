import java.util.*;

/*
Last box:
0 - loss
[1, K] - win
K + 1 - loss
[K + 2, 2K] - win
2K + 1

Last box:
x * K + 1 - loss
all others - win

2nd to last box:
[0, inf] 0 - loss
*/
public class tbuzzelli_compress {
  static final int ONLY_ME = 0;
  static final int ONLY_THEM = 1;
  static final int I_CHOOSE = 2;
  static final int THEY_CHOOSE = 3;
  static int k;
  static int[][] memo;

  public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    int n = in.nextInt();
    long origK = in.nextLong();
    k = (int) Math.min(4L, origK);
    memo = new int[n][50 * k + 1];
    for (int[] a : memo) {
      Arrays.fill(a, -1);
    }
    int[] arr = new int[n];
    for (int i = n - 1; i >= 0; i--) {
      long x = in.nextLong();
      int y = 0;
      long mod = x % (origK + 1);
      if (mod == 0) {
        y = 0;
      } else if (mod == 1) {
        y = 1;
      } else if (mod == origK) {
        y = k;
      } else {
        y = 2;
      }
      arr[i] = y;
    }
    int ans = solveMultiple(arr);
    System.out.println((ans == ONLY_ME || ans == THEY_CHOOSE) ? "Bertha" : "Aaron");
  }

  static int best(int a, int b) {
    if (a == b) {
      return a;
    }
    if (a == I_CHOOSE || b == I_CHOOSE) {
      return I_CHOOSE;
    }
    if (a == THEY_CHOOSE) {
      return b;
    }
    if (b == THEY_CHOOSE) {
      return a;
    }
    return I_CHOOSE;
  }

  static int combine(int a, int b) {
    switch (a) {
      case ONLY_ME:
        return b;
      case ONLY_THEM:
        return flip(b);
      default:
        return a;
    }
  }

  static int flip(int a) {
    return a ^ 1;
  }

  static int solveMultiple(int[] arr) {
    int ans = ONLY_ME;
    for (int i = 0; i < arr.length; i++) {
      ans = combine(ans, solveOne(i, arr[i]));
    }
    return ans;
  }

  // After cnt reaches 0, whose turn can it be?
  static int solveOne(int pos, int cnt) {
    if (pos < 0) {
      return ONLY_ME;
    }
    if (cnt == 0) {
      return ONLY_ME;
    }
    if (memo[pos][cnt] != -1) {
      return memo[pos][cnt];
    }
    int ans = -1;
    for (int x = 1; x <= k && x <= cnt; x++) {
      int tmp1 = solveOne(pos - 1, x);
      int tmp2 = solveOne(pos, cnt - x);
      int tmp = combine(flip(tmp1), tmp2);
      if (ans == -1) {
        ans = tmp;
      } else {
        ans = best(ans, tmp);
      }
    }
    return memo[pos][cnt] = ans;
  }
}
