#!/bin/bash

LIST=$(ls)

if [[ $2 -ne 0 ]]
then
    echo -e "SIZE\t\t\tCYCLES\n" > $1
fi

for FILE in $LIST ; do

    if [[ $FILE != out_file.* ]]
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
    echo -e $OUTPUT >> $1

done
