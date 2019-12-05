import json

for i in range(-1, 5):
    for j in range(-1, 5):
        for k in range(-1, 5):
            for l in range(-1, 5):
                f = open("hvalues/" + str(i) + "-" + str(j) + "-" + str(k) + "-" + str(l) + ".json", "w")
                json_string = {"FOUNDATION_SCORE":10*i,
                    "REVEAL_HIDDEN_SCORE":10*j,
                    "PLAN_REVEAL_HIDDEN_SCORE":10*k,
                    "EMPTY_SPACE_NO_KING_SCORE":10*l
                }
                json.dump(json_string, f)
