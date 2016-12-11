#!/bin/bash

let SIZE=$1
let FINAL=$2
let STEP=$3
let FLG=1

while [[ $SIZE -le $FINAL ]] ;
do

    ./matrix_generation.sh $SIZE $SIZE $STEP

    let TYPE=1

    while [[ $TYPE -le 6 ]] ;
    do
        ./matrix_calculation.sh $SIZE $SIZE $STEP $TYPE
 
        ./create_data_set.sh "main_out_file_$TYPE.txt" $FLG
 
        rm out_file_*.txt

        let TYPE=$TYPE+1

    done

    rm set_*.txt

    let FLG=0
    let SIZE=$SIZE+$STEP

done
