#!/bin/bash

lam=$1
N=$2
T=$3
dt=$4

let i=0
while [[ $i -ne 5 ]] ;
do
    ./main.out $lam $N $T $dt
    mv data.txt .experiment_$i.txt
    let i=$i+1
done

cp .experiment_0.txt result.txt

let i=1
while [[ $i -ne 5 ]] ;
do
    echo $i
    file1=$(cat result.txt)
    file2=$(cat .experiment_$i.txt)
    
    rm result.txt
    let j=0
    command="scale=0; 2*$T/$dt"
    cnt=$(bc <<< $command)

    for word in $file1 ;
    do
	array1[$j]=$word
	let j=$j+1
    done
    #echo ${array1[*]}
    let j=0
    
    for word in $file2 ;
    do
	array2[$j]=$word
	let j=$j+1
    done

    let j=0
    
    while [[ $j -ne $cnt ]] ;
    do
	let ind=$j+1
	v1=${array1[$ind]}
	v2=${array2[$ind]}
	command="scale=0; $v1+$v2"
	val=$(bc <<< $command)
	echo -e ${array1[$j]}'\t'$val >> result.txt
	#echo $val >> result.txt
	let j=$j+2
    done

    let i=$i+1
done

rm -rf .experiment*

let j=0

file3=$(cat result.txt)

for word in $file1 ;
do
    array3[$j]=$word
    let j=$j+1
done

echo ${array3[*]}

rm -rf result.txt

while [[ $j -ne $cnt ]] ;
do
    let ind=$j+1
    v1=${array3[$ind]}
    command="scale=0; $v1/5"
    val=$(bc <<< $command)
    echo -e ${array3[$j]}'\t'$val >> result.txt
    let j=$j+2
done

