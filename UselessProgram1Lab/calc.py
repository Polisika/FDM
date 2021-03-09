from math import log, sqrt


def calc(uh, uh2, u_ex):
    s1 = sum((a - b) ** 2 for a, b in zip(u_ex, uh)) / len(uh)
    s2 = sum((a - b) ** 2 for a, b in zip(u_ex, uh2)) / len(uh)
    
    return log(sqrt(s1) / sqrt(s2), 2)


n = 4
u_exact = []

u = [
    [],
    [],
    [],
    [],
    []
    ]
    
for i in range(len(u) - 1):
    print(f'p = {calc(u[i], u[i + 1], u_exact)}')
