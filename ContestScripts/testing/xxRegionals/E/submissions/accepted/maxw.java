import java.util.*;

public class maxw {
  public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    int N = in.nextInt();
    int[] e = new int[N];
    for (int i = 0; i < N; i++) e[i] = in.nextInt();
    TreeMap<Integer, Integer> last = new TreeMap<>();
    last.put(e[0], 0);
    for (int i = 1; i < N; i++) {
      Map.Entry<Integer, Integer> it = last.ceilingEntry(e[i]-i);
      if (it == null) it = last.lastEntry();
      int best = it.getValue();
      Map.Entry<Integer, Integer> p = last.lowerEntry(it.getKey());
      if (p != null) {
        if (Math.abs(p.getKey()-(e[i]-i)) < Math.abs(it.getKey()-(e[i]-i)))
          best = p.getValue();
        else if (Math.abs(p.getKey()-(e[i]-i)) == Math.abs(it.getKey()-(e[i]-i)))
          best = Math.max(best, p.getValue());
      }
      System.out.print(best+1 + " ");
      last.put(e[i]-i, i);
    }
    System.out.println();
  }
}
