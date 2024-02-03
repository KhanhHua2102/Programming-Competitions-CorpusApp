n = int(input())
A = sorted(list(map(int, input().split())))

sumL, sumR = A[0], sum(A[1:])
best = sumR - sumL

for i in range(1, n - 1):
    sumL += A[i]
    sumR -= A[i]
    best = max(best, sumR - sumL)

print(best)
