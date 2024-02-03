import java.util.*;

public class tbuzzelli_wa {
  public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    int n = in.nextInt();
    String ans = null;
    int best = -1;
    for (int i = 0; i < n; i++) {
      String t = in.next();
      int m = in.nextInt();
      int count = 0;
      for (int j = 0; j < m; j++) {
        String c = in.next();
        String p = in.next();
        String s = in.next();
        if (c.equals(s)) count++;
      }
      if (count > best) {
        ans = t;
        best = count;
      }
    }
    System.out.println(ans);
  }

}
