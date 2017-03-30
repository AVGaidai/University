#!/bin/bash

rm -rf result.txt

let i=4

while [[ $i -ne 14 ]] ;
do
    let j=0
    while [[ $j -ne 10 ]] ;
    do
	./main.out $i result.txt
	let j=$j+1
    done
    let i=$i+1
done

echo "# AVG time (10 starts)" > tmp.txt

let flg=0
let cnt=0
time="0"
for val in $(cat result.txt) ;
do
    if [[ flg -eq 0 ]] ;
    then
	let cnt=$cnt+1
	let flg=$val
	continue
    fi

    command="scale=3; $time+$val"
    time=$(bc <<< $command)
    if [[ cnt -eq 10 ]] ;
    then
	command="scale=3; $time/10"
	time=$(bc <<< $command)
	if [[ $(bc <<< "$time<1") -eq 1 ]] ;
	then
	    echo $flg "0"$time >> tmp.txt
	else
	    echo $flg $time >> tmp.txt
	fi
	let cnt=0
    fi
    let flg=0
done

rm -rf result.txt
mv tmp.txt result.txt

