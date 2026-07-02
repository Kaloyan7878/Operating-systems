#!/bin/bash
if [[ -z "$1" || -z "$2" ]]; then
    exit 1
fi
while read FILE; do
    OLD_DIR=$(realpath "$1")
    NEW_DIR=$(realpath "$2")
    NEWFILE=$(echo $(realpath $FILE) | sed "s|$OLD_DIR|$NEW_DIR|")
    while read LINE; do
        TOFIND=$(echo $LINE | awk -F '=' '{ print $1 }')
        INFO=$(echo $LINE | awk -F '=' '{ print $2 }')
        if ! grep "$TOFIND" "${NEWFILE}2" \
        2> /dev/null; then
            COUNT=$(grep "^${TOFIND}=" "$FILE" | wc -l)
            if [[ $COUNT -gt 1 ]]; then
                echo "${TOFIND}:" >> "${NEWFILE}2"
                grep "${TOFIND}=" $FILE | awk -F '=' '{ print "\t- " $2 }' >> "${NEWFILE}2"
            else
                echo "${TOFIND}: $INFO" >> "${NEWFILE}2"
            fi
        fi
    done < <(cat $FILE)
done < <(find $1 -name "*.bcf" -type f)
