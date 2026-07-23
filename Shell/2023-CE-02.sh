#!/bin/bash
#Разполагате с директория /var/log/my_logs, в която се намират log файлове. Log файл представлява обикновен файл, чието име има следния вид:

#<server_name>_<unix_timestamp>.log

#Името на сървър (server_name) може да съдържа букви, цифри и долни черти. Unix timestamp е число, означаващо брой изминали секунди 
#след полунощ на първи януари 1970. Считаме, че файловете в тази директория, чиито имена нямат този формат, не са log файлове.

#Пример за име: correlator_1692117813.log

#Напишете серия от команди, намираща общия брой на срещанията на думата error във всички log файлове.


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
