import java.util.*;

public class tbuzzelli {
  public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    int n = in.nextInt();
    PriorityQueue<Platform> platforms = new PriorityQueue<>((a, b) -> Integer.compare(a.l, b.l));
    PriorityQueue<Platform> pq = new PriorityQueue<>((a, b) -> Integer.compare(a.r, b.r));
    Platform cur = null;
    for (int i = 0; i < n; i++) {
      Platform p = new Platform(i + 1, in.nextInt(), in.nextInt());
      platforms.add(p);
      if (cur == null || p.r < cur.r) cur = p;
    }
    platforms.remove(cur);
    ArrayList<Integer> path = new ArrayList<>();
    while (true) {
      path.add(cur.id);
      while (!platforms.isEmpty()
          && platforms.peek().l <= cur.r
          && (pq.isEmpty() || pq.peek().r >= platforms.peek().l)) pq.add(platforms.poll());
      if (pq.isEmpty()) break;
      cur = pq.poll();
    }
    if (path.size() != n) {
      System.out.println("NO");
      return;
    }
    System.out.println("YES");
    StringBuilder sb = new StringBuilder();
    for (int i : path) {
      if (sb.length() > 0) sb.append(' ');
      sb.append(i);
    }
    System.out.println(sb.toString());
  }

  static class Platform {
    int id, l, r;

    Platform(int idd, int ll, int rr) {
      id = idd;
      l = ll;
      r = rr;
    }

    public boolean equals(Object o) {
      if (!(o instanceof Platform)) return false;
      return id == ((Platform) o).id;
    }
  }
}
