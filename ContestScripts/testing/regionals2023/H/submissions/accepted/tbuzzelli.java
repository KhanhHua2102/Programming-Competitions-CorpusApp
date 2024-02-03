import java.util.*;

public class tbuzzelli {
  static int n;
  static int[] skills;
  static double[] probs;
  static int[] misses;
  static double[] memo;

  public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    n = in.nextInt();
    skills = new int[n];
    probs = new double[n];
    misses = new int[n];
    for (int i = 0; i < n; i++) {
      skills[i] = in.nextInt();
    }
    for (int i = 0; i < n; i++) {
      probs[i] = in.nextDouble();
    }
    for (int i = 0; i < n; i++) {
      misses[i] = in.nextInt();
    }
    memo = new double[n];
    Arrays.fill(memo, -1.0);
    System.out.printf("%.12f\n", go(0));
  }

  static double go(int i) {
    if (i >= n) return 0;
    if (memo[i] != -1) return memo[i];
    return memo[i] =
        Math.max(go(i + 1), probs[i] * skills[i] + (1 - probs[i]) * go(i + 1 + misses[i]));
  }
}
