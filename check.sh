#!/bin/bash

ZERO=0

SCORE1=0.1
SCORE2=0.05
SCOREB=0

make clean
make build

EXEC=navigator
GRAPH=tests/graph.in

# MANDATORY
for EX in {1..7}
do
    IN_FILE="tests/c1-"$EX".in"
    USER_FILE="tests/c1-"$EX".user"
    OUT_FILE="tests/c1-"$EX".out"
    cat $IN_FILE | ./$EXEC 1 $GRAPH > $USER_FILE
    diff -wb $OUT_FILE $USER_FILE > /dev/null
    EXIT_CODE=$?
    if [[ $EXIT_CODE -eq $ZERO ]]
    then
        SCORE1=$(awk -v SCORE1=$SCORE1 'BEGIN { print SCORE1 + 0.7 }')
        echo "Testul 1-$EX trecut!"
    else
        echo "-0.7: Testul 1-$EX"
        echo "Testul 1-$EX picat!"
    fi
    rm -f $USER_FILE
done

for EX in {1..3}
do
    IN_FILE="tests/c2-"$EX".in"
    USER_FILE="tests/c2-"$EX".user"
    OUT_FILE="tests/c2-"$EX".out"
    cat $IN_FILE | ./$EXEC 2 $GRAPH > $USER_FILE
    diff -wb $OUT_FILE $USER_FILE > /dev/null
    EXIT_CODE=$?
    if [[ $EXIT_CODE -eq $ZERO ]]
    then
        SCORE2=$(awk -v SCORE2=$SCORE2 'BEGIN { print SCORE2 + 1.65 }')
        echo "Testul 2-$EX trecut!"
    else
        echo "-1.65: Testul 2-$EX"
        echo "Testul 2-$EX picat!"
    fi
done

for EX in {1..2}
do
    IN_FILE="tests/c3-"$EX".in"
    USER_FILE="tests/c3-"$EX".user"
    OUT_FILE="tests/c3-"$EX".out"
    cat $IN_FILE | ./$EXEC 3 $GRAPH > $USER_FILE
    diff -wb $OUT_FILE $USER_FILE > /dev/null
    EXIT_CODE=$?
    if [[ $EXIT_CODE -eq $ZERO ]]
    then
        SCOREB=$(awk -v SCOREB=$SCOREB 'BEGIN { print SCOREB + 1 }')
        echo "+1: Testul 3-$EX"
        echo "Testul 3-$EX trecut!"
    else
        echo "Testul 3-$EX picat!"
    fi
done

echo "Cerinta 1 : "$SCORE1
echo "Cerinta 2 : "$SCORE2
echo "BONUS     : "$SCOREB
