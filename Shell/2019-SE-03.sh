#!/bin/bash
#За удобство приемаме, че разполагате със системен инструмент sha256sum, който приема аргументи имена на файлове и за всеки файл
#пресмята и извежда уникална хеш стойност, базирана на съдържанието на файла. Изходът е текстови, по един ред за всеки подаден файл,4
#във формат: <хеш (64 знака)>  <име на файл>.

#Напишете скрипт, който приема задължителен параметър име на директория (ДИР1). Някъде в директорията ДИР1 може да съществуват
#архивни файлове с имена NAME_report-TIMESTAMP.tgz, където NAME е низ без _, а TIMESTAMP е Unix time.

#На всяко пускане на скрипта се обработват само новосъздадените или модифицираните по съдържание спрямо предното пускане на
#скрипта архивни файлове от горния тип. За всеки такъв архивен файл: ако архивният файл съдържа файл с име meow.txt,
#то този текстови файл да бъде записан под името /extracted/NAME_TIMESTAMP.txt.


if [[ -z "$1" ]]; then
    exit 1
fi
LOG_FILE="hash_sum.log"
TEMP_FILE="hash_sum.tmp"
> "$TEMP_FILE"
while IFS='\n' read -R FILE; do
    HASH_SUM=$(sha256sum $FILE)
    if [[ -n "$(grep ./hash_sum.log "$HASH_SUM")" ]]; then
        continue
    fi
    rm -rf "/tmp/my_temp_dir"
    mkdir -p "/tmp/my_temp_dir"
    tar -xf $FILE -C /tmp/my_temp_dir
    if [[ -n "$(find /tmp/my_temp_dir -name "meow.txt")" ]]; then
    
       NAME=$(basename $FILE | awk -F '_' '{ print $1 }')
       TIMESTAMP=$(basename $FILE | awk -F '-' '{ print $2 }' | \
       awk -F '.' '{ print $1 }')
       mkdir -p /extracted
       mv "/tmp/my_temp_dir/meow.txt" "/extracted/${NAME}_${TIMESTAMP}.txt"
    fi

    echo "$HASH_SUM  $FILE" >> "$TEMP_FILE"
done < <(find $1 -regextype posix-extended -regex ".*/[^_]*_report-[0-9]+\.tgz")
mv "$TEMP_FILE" "$LOG_FILE"
