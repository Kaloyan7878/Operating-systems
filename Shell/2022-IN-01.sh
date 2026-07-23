#!/bin/bash
#При отваряне на файл с редактора vi се създава в същата директория временен файл с име .<оригинално име>.swp.

#Напишете shell скрипт, който приема два задължителни позиционни аргумента — имена на директории (./foo.sh ./dir1 /path/to/dir2/).
#В dir1 може да има файлове/директории, dir2 трябва да е празна.

#Скриптът трябва да копира всички обикновени файлове от dir1 (и под-директориите ѝ) в dir2, запазвайки директорийната структура,
#но без да копира временните .swp файлове.

#Пример:

#dir1/            →      dir2/
#dir1/a                  dir2/a
#dir1/.a.swp             dir2/b
#dir1/b                  dir2/c/d
#dir1/c/d                dir2/c/.bar.swp
#dir1/c/.bar.swp


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
