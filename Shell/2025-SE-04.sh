#!/bin/bash
#Софтуерът BOMBA се конфигурира чрез текстови файлове с редове <ключ>=<стойност>. Пример:

#foo=the quick brown fox
#foo=jumps over the lazy dog
#bar=123456
#baz=baba penka
#bar=789

#Новият софтуер BOMBA2 групира ключове с повече от една стойност като списъци:

#foo:
#- the quick brown fox
#- jumps over the lazy dog
#bar:
#- 123456
#- 789
#baz: baba penka

#Напишете скрипт convert_to_bcf2.sh, който приема два аргумента (стара директория и нова директория). 
#Всеки обикновен файл в старата директория (и поддиректориите ѝ) с име, завършващо на .bcf, трябва:

#да се запише в новата директория, запазвайки директорийната структура, преименуван от <име>.bcf на <име>.bcf2
#да се преведе от стария конфигурационен синтаксис до новия
if [[ -z "$1" || -z "$2" ]]; then
    exit 1
fi
while read FILE; do
    OLD_DIR=$(realpath "$1")
    NEW_DIR=$(realpath "$2")
    NEWFILE=$(echo $(realpath $FILE) | sed "s|$OLD_DIR|$NEW_DIR|")
    while read LINE; do
        TOFIND=$(echo $LINE | awk -F '=' '{ print $1 }')
        INFO=$(echo $LINE | awk -F '=' '{ print $2 }')
        if ! grep "$TOFIND" "${NEWFILE}2" \
        2> /dev/null; then
            COUNT=$(grep "^${TOFIND}=" "$FILE" | wc -l)
            if [[ $COUNT -gt 1 ]]; then
                echo "${TOFIND}:" >> "${NEWFILE}2"
                grep "${TOFIND}=" $FILE | awk -F '=' '{ print "\t- " $2 }' >> "${NEWFILE}2"
            else
                echo "${TOFIND}: $INFO" >> "${NEWFILE}2"
            fi
        fi
    done < <(cat $FILE)
done < <(find $1 -name "*.bcf" -type f)
