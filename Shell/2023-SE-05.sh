#!/bin/bash
#Казваме, че командата foo заема памет X, когато X е сумата на заетата физическа памет (rss) на всички процеси, чиято команда (comm) е foo.

#Напишете скрипт, който поглежда процесите на системата през една секунда, докато спрат да съществуват команди, чиято заета памет е над 65536.

#След това, скриптът трябва да изведе всички команди, които са заемали памет над 65536 в поне половината "поглеждания".


FILE="/tmp/bad_commands.txt"
TOTAL_LOOKS=0
while true; do
    PREV_COMM=""
    CURR_SUM=0
    FOUND_BAD=0
    while read comm rss; do
        if [[ -n "$PREV_COMM" && "$comm" != "$PREV_COMM" ]]; then
            if [[ $CURR_SUM -gt 65536 ]]; then
                echo "$PREV_COMM" >> "$FILE"
                FOUND_BAD=1
            fi
            CURR_SUM=0
        fi
        (( CURR_SUM += $rss ))
        PREV_COMM=$comm
    done < <(ps -e -o comm=,rss= | sort -k 1)
    if [[ -n "$PREV_COMM" && $CURR_SUM -gt 65536 ]]; then
        echo "$PREV_COMM" >> "$FILE"
        FOUND_BAD=1
    fi
    if [[ $FOUND_BAD -eq 0 ]]; then
        break
    fi
    TOTAL_LOOKS=$(( TOTAL_LOOKS + 1 ))
    sleep 1
done
HALF=$(echo "$TOTAL_LOOKS / 2" | bc)
cat $FILE | sort | uniq -c | awk -v half="$HALF" '$1 >= half { print $2 }'
rm -f "$FILE"
