#!/bin/bash
#Напишете скрипт flatten.sh, който приема като аргумент име на директория (<DIR>) и:

#Премества всички обикновени файлове от директорията и поддиректориите ѝ в <DIR>/.data, преименувани на sha256 хеш-сумите на съдържанието им
#(без презаписване при вече съществуващ правилен хеш; .data се създава, ако липсва).
#Заменя оригиналните файлове със символни връзки, сочещи към съответния файл в .data.

#Изисквания: скриптът трябва да е идемпотентен (повторно изпълнение не създава проблем и не води до промяна), а символните връзки да не се чупят,
#ако <DIR> бъде преместена на друго място.


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
