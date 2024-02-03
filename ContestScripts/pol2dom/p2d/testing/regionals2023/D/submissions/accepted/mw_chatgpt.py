from collections import deque

def main():
    N, M = map(int, input().split())
    T = N-1
    is_source = [1]*N
    outgoing = [0]*M
    in_edge = [[] for _ in range(N)]
    incoming = [[] for _ in range(M)]
    
    for i in range(M):
        to, k, *from_nodes = map(int, input().split())
        to -= 1
        outgoing[i] = to
        is_source[to] = 0
        for j in range(k):
            frm = from_nodes[j] - 1
            incoming[i].append(frm)
            in_edge[frm].append(i)
    
    valid_edge = [1]*M
    par = [-1]*N

    def bfs():
        q = deque()
        rem = [len(incoming[i]) for i in range(M)]
        done = [0]*N
        order = []
        for i in range(N):
            if is_source[i]:
                q.append(i)
                done[i] = 1

        while q:
            at = q.popleft()
            if at == T:
                return order
            for i in in_edge[at]:
                if not valid_edge[i]:
                    continue
                if done[outgoing[i]]:
                    continue
                rem[i] -= 1
                if not rem[i]:
                    order.append(i)
                    q.append(outgoing[i])
                    done[outgoing[i]] = 1
                    par[outgoing[i]] = i

        return [-1]

    order = bfs()
    if order and order[0] == -1:
        print(-1)
        return

    valid_edge = [0]*M
    q = deque([T])
    done = [0]*N
    done[T] = 1
    while q:
        at = q.popleft()
        if par[at] == -1:
            continue
        valid_edge[par[at]] = 1
        for i in incoming[par[at]]:
            if not done[i]:
                q.append(i)
                done[i] = 1
    order = bfs()

    print(len(order))
    print(" ".join(str(i+1) for i in order))

if __name__ == "__main__":
    main()
