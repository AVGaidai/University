#!/bin/bash

COUNT="1"

let SIZE=$1
let FINAL=$2
let STEP=$3
let NODE=$4

while [ $SIZE -le $FINAL ] ; do

    echo $SIZE
    echo "#PBS -N out_file"            >  task_02.job
    echo "#PBS -l nodes=cn$NODE:ppn=8" >> task_02.job
    echo "#PBS -j oe"                  >> task_02.job
    echo " "                           >> task_02.job
    echo "cd \$PBS_O_WORKDIR"          >> task_02.job

    echo "./Mul_Matrix.out -s -n $SIZE -f set_$COUNT.txt" >> task_02.job

    qsub task_02.job

    let COUNT=$COUNT+1
    let SIZE=$SIZE+$STEP

done
