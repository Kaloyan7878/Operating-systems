#!/bin/bash
if [[ -z "$1" ]]; then
    exit 1
fi
if [[ "$(whoami)" == "root" ]]; then
    while read user; do
        SUM_RSS=$(ps -u "$user" -o rss= | awk 'BEGIN { sum=0 } { sum+=$1 } END  { print (sum ? sum : 0) }')     
        echo "$user $SUM_RSS"
        if [[ $SUM_RSS -gt $1 ]]; then
            MAX_ID=$(ps -u "$user" -o rss=,pid= | sort -k 1 -nr | head -n 1 | awk '{ print $2 }')
            kill -SIGTERM $MAX_ID
        fi
    done < <(ps -e -o user | sort | uniq)
else
    echo "NE e ot root"
    exit 2
fi
