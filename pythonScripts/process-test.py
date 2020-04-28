import pandas as pd
import numpy as np
import sys

def process_file(fp):
    df = pd.read_csv(fp, skiprows=1)
    df = df.transpose()
    df = df.drop("Unnamed: 0")
    df.index = df.index.str[1:]
    df["Win Percentage"] = np.diag(df)

    df = df.sort_values("Win Percentage", ascending=False)

    print("Top 10 Solvers")
    print(df["Win Percentage"].head(10))

if __name__ == "__main__":
    if len(sys.argv) > 1:
        process_file(sys.argv[1])
