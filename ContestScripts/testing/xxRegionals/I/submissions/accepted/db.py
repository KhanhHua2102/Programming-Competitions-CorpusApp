N = int(input())

best = (1, "")

for _ in range(N):
    name, len = input().split()
    count = 0
    for _ in range(int(len)):
        line = input()
        count -= line[0] == line[-1]
    best = min(best, (count, name))

print(best[1])
