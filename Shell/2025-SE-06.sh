#!/bin/bash
if [[ $# -ne 1 ]]; then
    exit 1
fi
if [[ -z "$1" ]]; then
    exit 1
fi
NEW_DIR="${1}/.data"
if [[ ! -d "$NEW_DIR" ]]; then
    mkdir "$NEW_DIR"
fi
while IFS= read FILE; do
    if [[ "$FILE" == *"/.data/"* ]]; then
        continue
    fi
    SHA_SUM=$(sha256sum "$FILE" | awk '{ print $1 }')
    if [[ -f "${NEW_DIR}/${SHA_SUM}" ]]; then
        rm "$FILE"
        ln -sr "${NEW_DIR}/${SHA_SUM}" "$FILE"
        continue
    fi
    mv "$FILE" "${NEW_DIR}/${SHA_SUM}"
    ln -sr "${NEW_DIR}/${SHA_SUM}" "$FILE"
done < <(find $1 -type f)
