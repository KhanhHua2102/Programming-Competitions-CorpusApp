import java.util.*;

public class tbuzzelli {
  public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    int n = in.nextInt();
    long s = in.nextInt();
    int h = in.nextInt();
    TreeMap<Long, Integer> tm = new TreeMap<>();
    for (int i = 0; i < n; i++) {
      long x = in.nextInt();
      tm.put(x, tm.getOrDefault(x, 0) + 1);
    }
    for (int i = 0; i < h; i++) {
      Long tmp = tm.lowerKey(s);
      if (tmp != null) {
        s += (int) Math.ceil(Math.sqrt(tmp));
        int cnt = tm.get(tmp);
        if (cnt == 1) {
          tm.remove(tmp);
        } else {
          tm.put(tmp, cnt - 1);
        }
      }
    }
    System.out.println(s);
  }
}
