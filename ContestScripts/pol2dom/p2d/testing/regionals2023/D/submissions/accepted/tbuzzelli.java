import java.util.*;

public class tbuzzelli {
  static int n, m;
  static Chest[] chests;

  public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    n = in.nextInt();
    m = in.nextInt();
    chests = new Chest[m];
    int[] needCounts = new int[m];
    boolean[] has = new boolean[n];
    ArrayList<Integer>[] lists = new ArrayList[n];
    for (int i = 0; i < n; i++) {
      lists[i] = new ArrayList<>();
    }
    Arrays.fill(has, true);
    for (int i = 0; i < m; i++) {
      int key = in.nextInt() - 1;
      has[key] = false;
      int k = in.nextInt();
      int[] need = new int[k];
      for (int j = 0; j < k; j++) {
        need[j] = in.nextInt() - 1;
        lists[need[j]].add(i);
      }
      chests[i] = new Chest(i + 1, key, need);
      needCounts[i] = k;
    }
    System.err.println(Arrays.toString(needCounts));
    ArrayDeque<Chest> q = new ArrayDeque<>();
    for (int i = 0; i < m; i++) {
      for (int need : chests[i].needs) {
        if (has[need]) {
          needCounts[i]--;
        }
      }
      if (needCounts[i] == 0) {
        q.add(chests[i]);
      }
    }
    List<Chest> ans = new ArrayList<>();
    while (!has[n - 1] && !q.isEmpty()) {
      Chest c = q.poll();
      if (has[c.key]) {
        continue;
      }
      ans.add(c);
      has[c.key] = true;
      for (int i : lists[c.key]) {
        if (--needCounts[i] == 0) {
          q.add(chests[i]);
        }
      }
    }
    if (!has[n - 1]) {
      System.out.println(-1);
      return;
    }
    ans = removeRedundantChests(ans);
    System.out.println(ans.size());
    for (Chest c : ans) {
      System.out.print(c.id + " ");
    }
    System.out.println();
  }

  static List<Chest> removeRedundantChests(List<Chest> chests) {
    List<Chest> res = new ArrayList<>();
    boolean[] needs = new boolean[n];
    int[] counts = new int[n];
    for (Chest c : chests) {
      counts[c.key]++;
    }
    needs[n - 1] = true;
    for (int ci = chests.size() - 1; ci >= 0; ci--) {
      Chest c = chests.get(ci);
      counts[c.key]--;
      if (!needs[c.key] || counts[c.key] > 0) {
        continue;
      }
      res.add(c);
      for (int need : c.needs) {
        needs[need] = true;
      }
    }
    Collections.reverse(res);
    return res;
  }

  static class Chest {
    int id;
    int key;
    int[] needs;

    public Chest(int id, int key, int[] needs) {
      this.id = id;
      this.key = key;
      this.needs = needs;
    }
  }
}
