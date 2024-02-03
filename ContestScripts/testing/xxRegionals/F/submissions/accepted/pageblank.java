import java.io.*;
import java.util.*;

public class pageblank {

  static StreamTokenizer st = new StreamTokenizer(new BufferedReader(new InputStreamReader(System.in)));
  static int readi() throws IOException {
    st.nextToken();
    return new Integer(st.sval).intValue();
  }

  // strongly connected component algorithm

  static final int WHITE = 0;
  static final int GRAY = 1;
  static final int BLACK = 2;
  static final int nmax = 1 << 20;

  static int n, ftime;
  static int[] color = new int[nmax];
  static int[] vertex = new int[nmax];
  static int[] comp = new int[nmax];
  static boolean[] sink = new boolean[nmax];
  static Vector<Vector<Integer>> adj = new Vector<Vector<Integer>>(), transp = new Vector<Vector<Integer>>();

  static void visit(int i) {
    color[i] = GRAY;
    for (int j : adj.elementAt(i))
      if (color[j] == WHITE)
        visit(j);
    color[i] = BLACK;
    vertex[ftime--] = i;
  }

  static Vector<Integer> scc;
  static void visit2(int j) {
    color[j] = GRAY;
    for (int i : transp.elementAt(j))
      if (color[i] == WHITE)
        visit2(i);
    color[j] = BLACK;
    scc.add(j);
  }

  // returns vector of components and stores each node's component in comp
  static Vector<Vector<Integer>> components() {
    ftime = n-1;
    for (int i=0 ; i<n ; i++)
      color[i] = WHITE;
    for (int i=0 ; i<n ; i++)
      if (color[i] == WHITE)
        visit(i);
    for (int i=0 ; i<n ; i++)
      color[i] = WHITE;
    Vector<Vector<Integer>> sccs = new Vector<Vector<Integer>>();
    for (int i=0 ; i<n ; i++)
      if (color[vertex[i]] == WHITE) {
        sccs.add(new Vector<Integer>());
        scc = sccs.lastElement();
        visit2(vertex[i]);
      }
    for (int i=0 ; i<sccs.size() ; i++)
      for (int j=0 ; j<sccs.elementAt(i).size() ; j++)
        comp[sccs.elementAt(i).elementAt(j)] = i;
    return sccs;
  }

  public static void main(String[] args) throws Exception {
    st.ordinaryChars('0', '9');
    st.wordChars('0', '9');
    n = readi();
    for (int i=0 ; i<n ; i++) {
      adj.add(new Vector<Integer>());
      transp.add(new Vector<Integer>());
    }
    for (int i=0 ; i<n ; i++) {
      int m = readi();
      for (int j=0 ; j<m ; j++) {
        int k = readi();
        adj.elementAt(i).add(k-1);
        transp.elementAt(k-1).add(i);
      }
    }
    // the solution contains all or no nodes of a strongly connected component
    Vector<Vector<Integer>> sccs = components();
    // if a component is in the solution, all its transitive successors are
    // find all components without successors (others cannot be optimal)
    for (int i=0 ; i<sccs.size() ; i++)
      sink[i] = true;
    for (int i=0 ; i<n ; i++)
      for (int j : adj.elementAt(i))
        if (comp[i] != comp[j])
          sink[comp[i]] = false;
    // components are disjoint
    // lexicographically smallest means the smallest identifier must be maximal
    int best=-1, smallest=-1;
    for (int i=0 ; i<sccs.size() ; i++) {
      if (sink[i]) {
        int s = Collections.min(sccs.elementAt(i));
        if (s > smallest) {
          best = i;
          smallest = s;
        }
      }
}
    Collections.sort(sccs.elementAt(best));
    System.out.print(sccs.elementAt(best).elementAt(0)+1);
    for (int i=1 ; i<sccs.elementAt(best).size() ; i++)
      System.out.print(" " + (sccs.elementAt(best).elementAt(i)+1));
    System.out.println();
  }
}

