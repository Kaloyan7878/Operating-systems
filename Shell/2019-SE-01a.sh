#!/bin/bash
ABS_MAX = 0
while IFS= read -r LINE; do
    if [[ -n $(grep -E "-[1-9]+|[1-9]+") ]]
        ABS_NUM = $(echo $(LINE) | grep -oE "-[0-9]+|[0-9]+" | \
        awk '{ //abs  }')
    
    fi
done
