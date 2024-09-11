#!/bin/bash
make clean
make benchmark

set -x

rm -f report.txt
for nreaders in 1 2 4 8 16; do
    ./benchmark $nreaders 1 10000000 1000
done;

