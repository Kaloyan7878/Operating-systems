#!/bin/bash
#proverki
TIME=$1
TIME_RUN=0
HOW_MANY=0
while (( $(echo "$TIME_RUN <= $TIME" | bc) )); do
    START_TIME=$(date +%s.%N)
    "${@:2}" > /dev/null 2>&1
    END_TIME=$(date +%s.%N)
    CURRENT_RUN=$(echo "$END_TIME - $START_TIME" | bc)
    TIME_RUN=$(echo "$TIME_RUN + $CURRENT_RUN" | bc)
    (( HOW_MANY++ ))
done
TIME_RUN=$(echo "scale=2; $TIME_RUN / 1" | bc)
AVERAGE_TIME=$(echo "scale=2; $TIME_RUN / $HOW_MANY" | bc)
echo "Ran the command ${@:2} $HOW_MANY times for $TIME_RUN seconds."
echo "Average runtime:  $AVERAGE_TIME seconds."
