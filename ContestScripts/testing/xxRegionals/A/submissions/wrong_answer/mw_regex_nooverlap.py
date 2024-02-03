import re
K = int(input().strip())
S = input().strip()
J = input().strip()

S = S.replace("$", "s")
J = J.replace("$", "s")

strs = []
for k in range(K+1):
    s = []
    for i in range(0, len(S), k+1):
        s.append(S[i])
    strs.append(''.join(s))

for str in strs:
    print(len(re.findall(J, str)))
