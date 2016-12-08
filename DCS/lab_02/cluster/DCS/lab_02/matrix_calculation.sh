#!/bin/bash

let COUNT=1
let SIZE=$1
let FINAL=$2
let STEP=$3
let TYPE=$4
let NODE=$5

while [ $SIZE -le $FINAL ] ; do

    echo "#PBS -N out_file"            >  task_01.job
    echo "#PBS -l nodes=cn$NODE:ppn=8" >> task_01.job
    echo "#PBS -j oe"                  >> task_01.job
    echo " "                           >> task_01.job
    echo "cd \$PBS_O_WORKDIR"          >> task_01.job

    echo "./Mul_Matrix.out -c $TYPE -f set_$COUNT.txt -n $SIZE" >> task_01.job

    qsub task_01.job

    let COUNT=$COUNT+1
    let SIZE=$SIZE+$STEP

done
