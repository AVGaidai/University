#!/bin/bash

let SIZE=$1
let FINAL=$2

let FLG=1

while [[ $SIZE -le $FINAL ]] ;
do

    ./matrix_generation.sh $SIZE $SIZE

    let NUM=0
    while [[ $NUM = 0 ]] ;
    do
        let NUM=$(find -name "out_file.*" | wc -l)
    done
    rm out_file.*

    let TYPE=1

    while [[ $TYPE -le 4 ]] ;
    do
        ./matrix_calculation.sh $SIZE $SIZE $TYPE
 
        let NUM=0
        while [[ $NUM = 0 ]] ;
        do
            let NUM=$(find -name "out_file.*" | wc -l)
        done
 
        ./create_data_set.sh "main_out_file_"$TYPE".txt" $FLG
 
        rm out_file.*

        let TYPE=$TYPE+1

    done

    rm set_*.txt
    FLG=0
    SIZE=$SIZE+16

done
