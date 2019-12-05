#!/bin/bash
for filename in hvalues/*.json; do
    ./source/server "-g" "1000" "-d" "3" "-s" "100" "-h" "$filename" ">>" "out.csv"
done
