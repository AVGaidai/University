#!/bin/bash

let COUNT=1
let SIZE=$1
let FINAL=$2

while [ $SIZE -le $FINAL ] ; do

    echo "#PBS -N out_file"      >  task_01.job
    echo "#PBS -l nodes=1:ppn=8" >> task_01.job
    echo "#PBS -j oe"            >> task_01.job
    echo " "                     >> task_01.job
    echo "cd \$PBS_O_WORKDIR"    >> task_01.job

    echo "./Mul_Matrix.out -c "$3" -f set_"$COUNT".txt -n $SIZE" >> task_01.job

    qsub task_01.job

    let COUNT=$COUNT+1
    let SIZE=$SIZE+16

done
