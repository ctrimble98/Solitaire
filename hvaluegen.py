import json

for i in range(-1, 3):
    for j in range(0, 4):
        for k in range(0, 4):
            for l in range(-1, 2):
                f = open("hvalues/" + str(i) + "-" + str(j) + "-" + str(k) + "-" + str(l) + ".json", "w")
                json_string = {
                    "FOUNDATION_SCORE": 10*i,
                    "REVEAL_HIDDEN_SCORE": 20,
                    "PLAN_REVEAL_HIDDEN_SCORE": 10,
                    "EMPTY_SPACE_NO_KING_SCORE": -10,
                    "STOCK_SAFE": 10*j,
                    "TABLEAU": 10*k,
                    "STOCK_DISTANCE": 0,
                    "SMOOTH": l
                }
                json.dump(json_string, f)
