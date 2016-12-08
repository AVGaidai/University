#!/bin/bash

let SIZE=$1
let FINAL=$2
let STEP=$3
let NODE=$4
let FLG=1

while [[ $SIZE -le $FINAL ]] ;
do

    ./matrix_generation.sh $SIZE $SIZE $STEP $NODE

    let NUM=0
    while [[ $NUM -eq 0 ]] ;
    do
        let NUM=$(find -name "out_file.*" | wc -l)
        sleep 1
    done
    rm out_file.*

    let TYPE=1

    while [[ $TYPE -le 4 ]] ;
    do
        ./matrix_calculation.sh $SIZE $SIZE $STEP $TYPE $NODE
 
        let NUM=0
        while [[ $NUM -eq 0 ]] ;
        do
            let NUM=$(find -name "out_file.*" | wc -l)
            sleep 2
        done
 
        ./create_data_set.sh "main_out_file_$TYPE.txt" $FLG
 
        rm out_file.*

        let TYPE=$TYPE+1

    done

    rm set_*.txt

    let FLG=0
    let SIZE=$SIZE+$STEP

done
