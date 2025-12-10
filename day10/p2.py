import pulp
import string

result = 0

def solve(s):
    global result
    print(s)
    splitted = s.split(' ')
    splitted = splitted[1:]
    pushes = []
    for r in splitted:
        r = r[1:-1]
        push = [int(x) for x in r.split(',')]
        pushes.append(push)
    
    target = pushes[-1]
    pushes = pushes[:-1]

    n = len(target)
    m = len(pushes)
    mat = [[0 for i in range(n)] for i in range(m)]
    for i in range(m):
        for push in pushes[i]:
            mat[i][push] = 1
    
    #print(mat)
                
    cols = [c for c in string.ascii_lowercase[:m]]
    vars = {name: pulp.LpVariable(name, lowBound=0, cat="Integer") for name in cols}
    prob = pulp.LpProblem("min", pulp.LpMinimize)
    prob += pulp.lpSum(vars[name] for name in cols), "sum"
    
    for i in range(n):
        prob += (pulp.lpSum(mat[j][i] * vars[cols[j]] for j in range(m)) == target[i]), f"eq_{i}"
    status = prob.solve()
    result += pulp.value(prob.objective)

f = open("actual.in", "r")
lines = f.readlines()
for line in lines:
    line = line.strip()
    if len(line) > 0:
        solve(line)

print(f"result: {result}")

