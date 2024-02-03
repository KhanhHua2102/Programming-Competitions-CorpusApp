import java.util.*;

public class tbuzzelli {
  static final boolean DEBUG = false;
  static int n;
  static int[] grundy;
  static XorTrie[] tries;
  static int[] pars;
  static ArrayList<Integer>[] adjs;
 
  public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    n = in.nextInt();
    grundy = new int[n + 1];
    tries = new XorTrie[n + 1];
    for (int i = 0; i <= n; i++) tries[i] = new XorTrie(Integer.highestOneBit(n * 4));
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
 
  static int getGrundy(int i) {
    if (grundy[i] != -1) return grundy[i];
    if (adjs[i].isEmpty()) {
      tries[i].add(0);
      return grundy[i] = 1;
    }
    int xor = 0;
    for (int next : adjs[i]) xor ^= getGrundy(next);
    for (int next : adjs[i]) tries[i] = XorTrie.merge(tries[i], tries[next].xor(xor ^ getGrundy(next)));
    if (i != 0) tries[i].add(xor);
    return grundy[i] = tries[i].mex();
  }

  static class XorTrie {
    int bit, count, xor;
    XorTrie[] children = new XorTrie[2];
    TreeSet<Integer> set = DEBUG ? new TreeSet<>() : null;

    XorTrie(int bit) {
      this.bit = bit;
    }

    boolean add(int x) {
      x ^= xor;
      if (DEBUG) set.add(x);
      if (bit != 0) {
        int idx = (x & bit) == 0 ? 0 : 1;
        if (children[idx] == null) children[idx] = new XorTrie(bit >> 1);
        boolean res = children[idx].add(x);
        if (res) count++;
        return res;
      }
      boolean res = count == 0;
      count = 1;
      return res;
    }

    int mex() {
      if (DEBUG) {
        System.err.println("\n\nFinding the mex of: " + getDebugSet());
        int res = mex(0, xor);
        System.err.println("Got " + res);
        return res;
      }
      return mex(0, xor);
    }

    XorTrie xor(int xor) {
      this.xor ^= xor;
      return this;
    }

    TreeSet<Integer> getDebugSet() {
      TreeSet<Integer> res = new TreeSet<>();
      for (int i : set) res.add(i ^ xor);
      return res;
    }

    // May merge a into b or b into a. Returns the XorTrie that
    // has been merged into.
    static XorTrie merge(XorTrie a, XorTrie b) {
      if (a.count < b.count) return merge(b, a);
      if (DEBUG) {
        System.err.printf("\n\nMerging %s and %s\n", a.getDebugSet(), b.getDebugSet());
      }
      b.merge(a, b.xor, 0);
      if (DEBUG) {
        System.err.printf("Got %s\n", a.getDebugSet());
      }
      return a;
    }

    private void merge(XorTrie into, int xor, int path) {
      if (bit == 0) into.add(path ^ xor);
      if (children[0] != null) children[0].merge(into, xor, path);
      if (children[1] != null) children[1].merge(into, xor, path ^ bit);
    }

    private int mex(int path, int xor) {
      if (DEBUG) System.err.printf("  path: %d  xor: %d  bit: %d  count(0): %d  count(1): %d\n", path, xor, bit, children[0] == null ? 0 : children[0].count, children[1] == null ? 0 : children[1].count);
      if (count >= bit * 2) return -1;
      int idx = (xor & bit) == 0 ? 0 : 1;
      if (children[idx] == null) return path;
      int tmp = children[idx].mex(path, xor);
      if (tmp != -1) return tmp;
      idx ^= 1;
      path ^= bit;
      if (children[idx] == null) return path;
      return children[idx].mex(path, xor);
    }
  }

}
