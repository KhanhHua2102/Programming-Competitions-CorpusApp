import sys
import threading
threading.stack_size(2**27-2**12)
sys.setrecursionlimit(10**6)

def main():
  global ftime
  WHITE, GRAY, BLACK = 0, 1, 2

  def visit(i):
    global ftime
    color[i] = GRAY
    for j in adj[i]:
      if color[j] == WHITE:
        visit(j)
    color[i] = BLACK
    vertex[ftime] = i
    ftime -= 1

  def visit2(scc, j):
    color[j] = GRAY
    for i in transp[j]:
      if color[i] == WHITE:
        visit2(scc, i)
    color[j] = BLACK
    scc.append(j)

  n = int(sys.stdin.readline().strip())
  adj = [ [ int(x)-1 for x in sys.stdin.readline().split()[1:] ] for _ in range(n) ]
  transp = [ [] for _ in range(n) ]
  for i in range(n):
    for j in adj[i]:
      transp[j].append(i)
  ftime = n-1
  color = [WHITE]*n
  vertex = [0]*n
  for i in range(n):
    if color[i] == WHITE:
      visit(i)
  color = [WHITE]*n
  sccs = []
  for i in range(n):
    if color[vertex[i]] == WHITE:
      sccs.append([])
      visit2(sccs[-1], vertex[i])
  comp = [0]*n
  for i, scc in enumerate(sccs):
    for j in scc:
      comp[j] = i
  sink = [ True for _ in sccs ]
  for i in range(n):
    for j in adj[i]:
      if comp[i] != comp[j]:
        sink[comp[i]] = False
  best = smallest = -1
  for i, scc in enumerate(sccs):
    if sink[i]:
      s = min(scc)
      if s > smallest:
        best, smallest = i, s
  print(' '.join(str(x+1) for x in sorted(sccs[best])))

threading.Thread(target=main).start()
