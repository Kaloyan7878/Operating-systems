#!/bin/bash
#Напишете скрипт orange-cat.sh, който приема път до файл като аргумент и изписва съдържанието му на stdout, вмъквайки съдържания на други файлове,
#когато срещне редове, започващи с !include.

#Вместо всеки ред !include: <път до файл2>, скриптът трябва да изпише съдържанието на файл2.
#Скриптът трябва да работи рекурсивно (вложени !include директиви също се заменят).
#Пътищата са релативни спрямо директорията на файла, съдържащ съответната !include директива.
if [[ $# -ne 1 || ! -f "$1" ]]; then
    exit 1
fi
function process {
    local curr_file="$1"
    local curr_dir=$(dirname $curr_file)
    while read -r LINE; do
        if [[ "$LINE" =~ ^!include: (.+) ]]; then
            local include_file=$(echo "$LINE" | awk '{ print $2 }')
            local next_file="$curr_dir/$include_file"
            if [[ -f "$next_file" ]]; then
                process "$next_file"
            else
                echo "FILE DOESNT EXIST"
            fi
        else
            echo "$LINE"
        fi
    done < <(cat $curr_file)
}
process "$1"
