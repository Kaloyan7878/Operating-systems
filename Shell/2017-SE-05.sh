#!/bin/bash
if [[ ! $# -eq 2 ]]; then
    exit 1
fi
if [[ -n "$1" || -n "$2" ]];then
    exit 1
fi

TOFIND=$(find $1 -maxdepth 1 -regextype posix-extended -regex \
".*/vmlinuz-[0-9]+\.[0-9]+\.[0-9]+-$2" | cut -c 9- \
| sed "s/-$2//" | sort -v | tail -n 1)
echo "vmlixnczn-$TOFIND-$2"
