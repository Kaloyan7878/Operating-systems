#!/bin/bash
if [[ -z "$1" ]]; then
    exit 1
fi
LOG_FILE="hash_sum.log"
TEMP_FILE="hash_sum.tmp"
> "$TEMP_FILE"
while IFS='\n' read -R FILE; do
    HASH_SUM=$(sha256sum $FILE)
    if [[ -n "$(grep ./hash_sum.log "$HASH_SUM")" ]]; then
        continue
    fi
    rm -rf "/tmp/my_temp_dir"
    mkdir -p "/tmp/my_temp_dir"
    tar -xf $FILE -C /tmp/my_temp_dir
    if [[ -n "$(find /tmp/my_temp_dir -name "meow.txt")" ]]; then
    
       NAME=$(basename $FILE | awk -F '_' '{ print $1 }')
       TIMESTAMP=$(basename $FILE | awk -F '-' '{ print $2 }' | \
       awk -F '.' '{ print $1 }')
       mkdir -p /extracted
       mv "/tmp/my_temp_dir/meow.txt" "/extracted/${NAME}_${TIMESTAMP}.txt"
    fi

    echo "$HASH_SUM  $FILE" >> "$TEMP_FILE"
done < <(find $1 -regextype posix-extended -regex ".*/[^_]*_report-[0-9]+\.tgz")
mv "$TEMP_FILE" "$LOG_FILE"
