#!/bin/bash
if [[ $# -ne 1 || ! -f "$1" ]]; then
    exit 1
fi
function process {
    local curr_file="$1"
    local curr_dir=$(dirname $curr_file)
    while read -r LINE; do
        if [[ "$LINE" =~ ^!include: (.+) ]]; then
            local include_file=$(echo "$LINE" | awk '{ print $2 }')
            local next_file="$curr_dir/$include_file"
            if [[ -f "$next_file" ]]; then
                process "$next_file"
            else
                echo "FILE DOESNT EXIST"
            fi
        else
            echo "$LINE"
        fi
    done < <(cat $curr_file)
}
process "$1"
