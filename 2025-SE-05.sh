#!/bin/bash
if [[ -z "$1" || ! $# -eq 1 ]]; then
    exit 1
fi
if [[ ! -d "$1" ]]; then
    exit 1
fi
if [[ $(whoami) == "root" ]]; then
    find $1 -type d -exec chmod g+rwx,g+s,o-rwx {} +
    find $1 -type f -exec chmod g+rw,o-rwx {} +
elif ! id -G | grep -q -w "$(stat -c '%g' "$1")"; then
    exit 1
else
    umask 007
fi

