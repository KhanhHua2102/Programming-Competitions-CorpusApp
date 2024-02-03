import java.util.*;

public class tbuzzelli_infinite_loop {

  public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    int Q = in.nextInt();
    while(Q-- > 0) {
        List<String> toks = new ArrayList<>();
        boolean[] seen = new boolean[256];
        for(int i=0;i<4;i++) {
          String str = in.next();
          toks.add(str);
          seen[str.charAt(0)] = true;
        }
        if (!seen['+']) {
          toks.add("+0");
        }
        if (!seen['-']) {
          toks.add("-0");
        }
        if (!seen['*']) {
          toks.add("*1");
        }
        if (!seen['/']) {
          toks.add("/1");
        }
        char[] ops = new char[toks.size()];
        long[] vals = new long[toks.size()];
        for (int i = 0; i < toks.size(); i++) {
          ops[i] = toks.get(i).charAt(0);
          vals[i] = Long.parseLong(toks.get(i).substring(1));
        }
        Equation e = new Equation(ops, vals);
        long ans = e.getAns();
        System.out.println(ans == -1 ? "BAD CODE" : ans);
    }
  }

  static class Equation {
    char[] ops;
    long[] vals;

    public Equation(char[] ops, long[] vals) {
      this.ops = ops;
      this.vals = vals;
    }

    long eval(long x) {
      return eval(x, 0);
    }

    long undoShift(long x) {
      return eval(x, divIndex() + 1);
    }

    long eval(long x, int start) {
      if (x < 0) {
        return -1;
      }
      for (int i = start; i < 4; i++) {
        if (ops[i] == '+') {
          x += vals[i];
        } else if (ops[i] == '-') {
          x -= vals[i];
        } else if (ops[i] == '*') {
          x *= vals[i];
        } else if (ops[i] == '/') {
          if (vals[i] == 0) {
            return -1;
          }
          x /= vals[i];
        }
        if (x < 0) {
          return -1;
        }
      }
      return x;
    }

    long getAns() {
      long fp = getSmallestFixedPoint();
      // System.err.println(fp);
      if (fp == -1) {
        return -1;
      }
      long min = 0, max = fp;
      while (min < max) {
        long mid = (min + max) / 2;
        // System.err.printf("[%d, %d] %d: %d\n", min, max, mid, eval(mid));
        if (eval(mid) < mid) {
          min = mid + 1;
        } else {
          max = mid;
        }
      }
      return min;
    }

    long getSmallestFixedPoint() {
      long[] fps = getShiftedFixPointRange();
      if (fps == null) {
        return -1;
      }
      // System.err.println(Arrays.toString(fps));
      long min = fps[0], max = fps[1];
      while (min < max) {
        long mid = (min + max) / 2;
        if (eval(undoShift(mid)) >= 0) {
          max = mid;
        } else {
          min = mid + 1;
        }
      }
      return eval(undoShift(min));
    }

    int divIndex() {
      for (int i = 0; i < 4; i++) {
        if (ops[i] == '/') {
          return i;
        }
      }
      throw new RuntimeException("Divide not found!");
    }

    long[] getShiftedFixPointRange() {
      int di = divIndex();
      long a = 1, b = 0, c = vals[di];
      for (int i = 1; i <= 3; i++) {
        // System.err.printf("(%dx + %d) / %d = x\n", a, b, c);
        int j = (di + i) % 4;
        if (ops[j] == '*') {
          a *= vals[j];
          b *= vals[j];
        } else if (ops[j] == '+') {
          b += vals[j];
        } else {
          b -= vals[j];
        }
      }
      // System.err.printf("(%dx + %d) / %d = x\n", a, b, c);
      // (ax + b) / c = x
      // floor((ax + b - cx) / c) = 0
      // (a - c) * x + b = [0, c - 1]
      // = 0:
      // (a - c) * x + b = 0
      // -b / (a - c)
      // c-1:
      // (a - c) * x + b = c - 1
      // (c - 1 - b) / (a - c)
      long[] res = null;
      if (a == c) {
        if (b < 0 || b >= c) {
          return null;
        }
        // TODO: Figure out what infinity to put here.
        return new long[] {0, 1_000_000_000_000L};
      }
      long den = a - c;
      long num1 = -b;
      long num2 = c - 1 - b;
      if (den < 0) {
        den *= -1;
        num1 *= -1;
        num2 *= -1;
      }
      long min = Math.floorDiv(Math.min(num1, num2) + den - 1, den);
      long max = Math.floorDiv(Math.max(num1, num2), den);
      // System.err.println(min + " " + max);
      if (min > max) {
        return null;
      }
      return new long[] {min, max};
    }
  }
}
