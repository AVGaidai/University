#!/bin/bash

LIST=$(ls)
FNAME=$1

if [[ $2 -ne 0 ]]
then
    echo -e "#SIZE\t\t\tCYCLES\n" > $FNAME
fi

for FILE in $LIST ; do

    if [[ $FILE != out_file_*.txt ]]
    then
        continue
    fi

    CONTENT=$(cat $FILE)
    OUTPUT=""
    for WORD in $CONTENT ; do

        if [[ $WORD = "SIZE" || $WORD = "CYCLES" ]]
        then
            continue
        else
            OUTPUT=$OUTPUT$WORD"\t\t\t"
        fi

    done
    echo -e $OUTPUT >> $FNAME

done
