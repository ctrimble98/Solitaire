import json

f = open("solverConfigs/heuristic-test-values.json", "w")

solvers = []

for i in range(-1, 3):
    for j in range(0, 4):
        for k in range(0, 4):
            for l in range(0, 2):
                for m in range(-1, 2):
                    solvers.append({
                        "name": str(i) + "/" + str(j) + "/" + str(k) + "/" + str(l) + "/" + str(m),
                        "heuristics": {
                            "FOUNDATION": 10*i,
                            "REVEAL_HIDDEN": 20,
                            "PLAN_REVEAL_HIDDEN": 10,
                            "EMPTY_SPACE_NO_KING": -10,
                            "STOCK_SAFE": 5*j,
                            "TABLEAU": 5*k,
                            "STOCK_DISTANCE": 1*l,
                            "SMOOTH": m
                        }
                    })

json_string = {
    "solvers": solvers
}
json.dump(json_string, f)
