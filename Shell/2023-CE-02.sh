#!/bin/bash
if [[ ! $# -eq 1 ]]; then
    exit 1
fi
if [[ -z "$1" ]]; then
    exit 1
fi
ERROR_COUNT=0
while read -r FILE; do
    ERROR_COUNT_FILE=$(cat $FILE | grep -oF "error" | wc -l)
    (( ERROR_COUNT += $ERROR_COUNT_FILE ))
done < <(find $1 -regextype posix-extended -regex ".*[a-zA-Z0-9_]*_[0-9]+\.log")
echo "$ERROR_COUNT"
