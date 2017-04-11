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
    while [[ $j -ne $((2*$T/$dt) ]] ;
    do
	echo $file1[$j] >> result.txt
	echo -e "\t" >> result.txt
	echo $(($file1[$j+1]+$file2[$j+1])) >> result.txt
	echo -e "\n" >> result.txt

	let j=$j+2
    done
done

file1=$(cat result.txt)
rm result.txt
let j=0
while [[ $j -ne $((2*$T/$dt) ]] ;
do
    echo $file1[$j] >> result.txt
    echo -e "\t" >> result.txt
    echo $(($file1[$j+1]/10)) >> result.txt
    echo -e "\n" >> result.txt
    let j=$j+2
done


