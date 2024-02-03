n = int(input())
A = sorted(list(map(int, input().split())))

me = list(filter(lambda x: x >= 0, A))
you = list(filter(lambda x: x < 0, A))

if len(me) == 0:
    me.append(you.pop())

if len(you) == 0:
    you.append(me.pop(0))

print(sum(me) - sum(you))
