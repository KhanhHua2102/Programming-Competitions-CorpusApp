import java.util.*;

public class tbuzzelli {
  public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    int n = in.nextInt();
    long limit = in.nextLong();
    SegmentTree st = new SegmentTree(n + 1);
    ArrayDeque<Integer> stack = new ArrayDeque<>();
    int left = 0;
    long sum = 0;
    long[] vs = new long[n];
    long[] ss = new long[n];
    long prev = 0;
    for (int i = 0; i < n; i++) {
      vs[i] = in.nextLong();
      ss[i] = in.nextLong();
      sum += ss[i];
      while (sum > limit) sum -= ss[left++];
      st.update(i, i, vs[i]);
      while (!stack.isEmpty() && vs[i] >= vs[stack.peek()]) {
        int right = stack.poll();
        st.update(stack.isEmpty() ? 0 : stack.peek() + 1, right, vs[i] - vs[right]);
      }
      // System.err.println("Valid range: [" + left + ", " + i + "]");
      st.update(i + 1, i + 1, st.queryMin(left, i));
      // System.err.println(st.queryMin(left, i));
      stack.push(i);
    }
    System.out.println(st.queryMin(n, n));
  }

  static class SegmentTree {
    long[] min, delta;
    int n;
    
    public SegmentTree(int n) {
      this.n = n;
      min = new long[n * 4 + 1];
      delta = new long[n * 4 + 1];
    }
    
    void prop(int i) {
      set(i * 2, delta[i]);
      set(i * 2 + 1, delta[i]);
      delta[i] = 0;
    }
    
    void set(int i, long value) {
      delta[i] += value;
      min[i] += value;
    }
    
    void update(int left, int right, long value) {
      // System.err.printf("Updating [%d, %d] %d\n", left, right, value);
      update(1, 0, n-1, left, right, value);
    }
    
    void update(int i, int lr, int rr, int left, int right, long value) {
      if (lr == left && rr == right)
        set(i, value);
      else {
        prop(i);
        int mid = (lr + rr) / 2;
        if (left <= mid)
          update(i * 2, lr, mid, left, Math.min(mid,  right), value);
        if (mid < right)
          update(i * 2 + 1, mid + 1, rr, Math.max(mid + 1, left), right, value);
        min[i] = Math.min(min[i * 2], min[i * 2 + 1]);
      }
    }
    
    long queryMin(int left, int right) {
      return query(1, 0, n-1, left, right);
    }
    
    long query(int i, int lr, int rr, int left, int right) {
      if (lr == left && rr == right)
        return min[i];
      else {
        prop(i);
        int mid = (lr + rr) / 2;
        long ret = Long.MAX_VALUE;
        if (left <= mid) {
          long l = query(i * 2, lr, mid, left, Math.min(mid, right));
          ret = Math.min(ret, l);
        }
        if (mid < right) {
          long r = query(i * 2 + 1, mid + 1, rr, Math.max(mid + 1, left), right);
          ret = Math.min(ret, r);
        }
        return ret;
      }
    }
  }
}
