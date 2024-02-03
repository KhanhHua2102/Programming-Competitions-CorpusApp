import java.util.*;

public class tbuzzelli {
  static String first, middle, last;

  public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    long k = in.nextLong();
    long c = in.nextLong();
    first = in.next();
    middle = in.next();
    last = in.next();
    System.out.println(getChar(k, c));
  }

  static char getChar(long k, long c) {
    long fLen = 3 * k * (k + 1) / 2 + first.length() * (k + 1);
    if (c < fLen) {
      long min = 0, max = k;
      while (min < max) {
        long mid = (min + max) / 2;
        long midFLen = 3 * mid * (mid + 1) / 2 + first.length() * (mid + 1);
        long before = fLen - midFLen;
        if (c < before) {
          min = mid + 1;
        } else {
          max = mid;
        }
      }
      long tmpFLen = 3 * min * (min + 1) / 2 + first.length() * (min + 1);
      long before = fLen - tmpFLen;
      c -= before;
      if (c < first.length()) {
        return first.charAt((int) c);
      }
      c -= first.length();
      return "SON".charAt((int) (c % 3));
    }
    c -= fLen;
    if (c < middle.length()) {
      return middle.charAt((int) c);
    }
    c -= middle.length();
    return last.charAt((int) (c % last.length()));
  }
}
