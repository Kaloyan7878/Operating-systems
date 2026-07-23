#!/bin/bash
#Напишете скрипт, който получава задължителен първи позиционен параметър – директория и незадължителен втори – число. 
#Скриптът трябва да проверява подадената директория и нейните под-директории и да извежда имената на:

#а) при подаден на скрипта втори параметър – всички файлове с брой hardlink-ове поне равен на параметъра;

#б) при липса на втори параметър – всички symlink-ове с несъществуващ destination (счупени symlink-ове).

#Забележка: За удобство приемаме, че ако има подаден втори параметър, то той е число.

if [[ -z $1 ]]; then
    exit 1
fi
if [[ $# -gt 2 ]]; then
    exit 1
fi
if [[ $# -eq 2 ]]; then
    while read -r LINE; do
        HARDLINKS_COUNT=$(echo $LINE | awk '{ print $2 }')
        FILE_NAME=$(echo $LINE | awk '{ print $1 }')
        if [[ $HARDLINKS_COUNT -eq $2 || $HARDLINKS_COUNT -gt $2 ]]; then
            echo "$FILE_NAME"
        fi
    done < <(find $1 -type f -exec stat -c "%n %h" {} \;)
else
    while read -r LINE; do
        FILE_NAME=$(stat $LINE | head -n 1 | cut -c 9- | awk -F ' -> ' \
        '{ print $2 }')
        [[ ! -e $FILE_NAME ]] && echo "$LINE"
    done < <(find $1 -type l)
fi
