#!/bin/bash

PROG=../build/genetic
INSTDIR=../inst
N=30
PM=0.4

for F in "$INSTDIR"/*.fsp; do
  NOMBRE=$(basename "$F" .fsp)
  for s in $(seq 1 10); do
    echo -n "$NOMBRE GA semilla=$s -> "
    $PROG "$F" --alg ga --N $N --pm $PM --seed $s | grep Cmax
  done
done