#!/bin/bash
if [[ -z "$1" || -z "$2" ]]; then
    exit 1
fi
if [[ ! $(find "$2" -mindepth 1 | wc -l) -eq 0 ]]; then
    exit 1
fi
while read -r FILEPATH; do
    DEST=$(echo "$FILEPATH" | sed "s|$1|$2|")
    DEST_DIR=$(dirname $DEST)
    if basename $FILEPATH | grep -qE "\..*\.swp"; then
        TOFIND=$(basename $FILEPATH | awk -F '.' '{ print $2 }')
        if find $1 -type f -printf "%f\n" | grep -qF "$TOFIND"; then
            continue
        else
            mkdir -p $DEST_DIR
            cp $FILEPATH $DEST
        fi
    else
        mkdir -p $DEST_DIR
        cp $FILEPATH $DEST
    fi
done < <(find "$1" -type f)
