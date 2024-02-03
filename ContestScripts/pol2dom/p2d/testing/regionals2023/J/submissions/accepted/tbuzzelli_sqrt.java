import java.util.*;

public class tbuzzelli_tle {
  static String first, middle, last;
  static int n1, n2, n3;

  public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    long k = in.nextLong();
    long c = in.nextLong();
    first = in.next();
    middle = in.next();
    last = in.next();
    n1 = first.length();
    n2 = middle.length();
    n3 = last.length();
    System.out.println(getChar(k, c));
  }

  static char getChar(long k, long c) {
    while (true) {
      if (k == 0) {
        return (first + middle + last).charAt((int) c);
      }
      if (c < n1) {
        return first.charAt((int) c);
      }
      long[] lens = getLengths(k);
      if (c < lens[0]) {
        return "SON".charAt((int) ((c - n1) % 3));
      }
      c -= lens[0];
      if (c < lens[1]) {
        long allFirsts = n1 * k + 3 * k * (k - 1) / 2;
        if (c < allFirsts) {
          // Maybe special case this part.
        }
        k--;
        continue;
      }
      c -= lens[1];
      return last.charAt((int) c);
    }
  }

  static long[] getLengths(long k) {
    long firstLength = n1 + 3 * k;
    long middleLength = n1 * k + 3 * k * (k - 1) / 2 + n2 + n3 * k;
    long lastLength = n3;
    return new long[] {
      firstLength, middleLength, lastLength, firstLength + middleLength + lastLength
    };
  }
}
