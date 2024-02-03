import java.util.*;

public class tbuzzelli_n2 {
  static int n;
  static int[] grundy;
  static int[] pars;
  static ArrayList<Integer>[] adjs;

  public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    n = in.nextInt();
    grundy = new int[n + 1];
    adjs = new ArrayList[n + 1];
    pars = new int[n + 1];
    for (int i = 0; i <= n; i++) adjs[i] = new ArrayList<>();
    for (int i = 1; i <= n; i++) {
      pars[i] = in.nextInt();
      adjs[pars[i]].add(i);
    }
    Arrays.fill(grundy, -1);
    System.out.println(getGrundy(0) != 0 ? "Jay" : "ja$on");
  }

  static void dfs(int cur, int xor, ArrayList<Integer> nums) {
    for (int next : adjs[cur]) {
      xor ^= getGrundy(next);
    }
    if (cur != 0) nums.add(xor);
    for (int next : adjs[cur]) {
      dfs(next, xor ^ getGrundy(next), nums);
    }
  }

  static int getGrundy(int i) {
    if (grundy[i] != -1) return grundy[i];
    ArrayList<Integer> nums = new ArrayList<>();
    dfs(i, 0, nums);
    Collections.sort(nums);
    int min = 0;
    for (int num : nums)
      if (num == min) min++;
      else if (num > min) break;
    return grundy[i] = min;
  }
}
