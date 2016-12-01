#!/bin/bash

COUNT="1"

let SIZE=16

while [ $SIZE -le 4096 ] ; do
    echo $SIZE
    ./Mul_Matrix.out -s -n $SIZE -f "set_"$COUNT".txt"
    let COUNT=$COUNT+1
    let SIZE=$SIZE+16
done
