#!/bin/bash

lam=$1
N=$2
T=$3
dt=$4

let i=0
while [[ $i -ne 10 ]] ;
do
    ./main.out $lam $N $T $dt
    mv data.txt .experiment_$i.txt
    let i=$i+1
done

cp .experiment_0.txt result.txt

let i=1
while [[ $i -ne 10 ]] ;
do
    file1=$(cat result.txt)
    file2=$(cat .experiment_$i.txt)

    rm result.txt
    let j=0
    command="scale=0; 2*$T/$dt"
    cnt=$(bc <<< $command)
    while [[ $j -ne $cnt ]] ;
    do
	v1=$file1[$j+1]
	v2=$file2[$j+1]
	echo $v1  $v2
	command="scale=0; $v1+$v2"
	val=$(bc <<< $command)
	echo -e $file1[$j]'\t'$val >> result.txt
	
	let j=$j+2
    done
done

file1=$(cat result.txt)
rm result.txt
let j=0
while [[ $j -ne $cnt ]] ;
do
    echo -e $file1[$j]\t$(($file1[$j+1]/10)) >> result.txt
    let j=$j+2
done
rm -rf .experiment*
