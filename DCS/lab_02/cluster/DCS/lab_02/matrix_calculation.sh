#!/bin/bash

let COUNT=1
let SIZE=$1
let FINAL=$2
let STEP=$3
let TYPE=$4

while [ $SIZE -le $FINAL ] ; do

    ./Mul_Matrix.out -c $TYPE -f set_$COUNT.txt -n $SIZE -o out_file_$COUNT.txt

    let COUNT=$COUNT+1
    let SIZE=$SIZE+$STEP

done
