#!/bin/bash
#Създайте скрипт reporter.sh, който приема един или повече пътища до директории. Скриптът трябва да обходи всички обикновени файлове
#в дадените директории (и поддиректориите им), за да създаде доклад.

#Очакват се environment променливите:

#REGISTRY_FILE — път до файл-регистър (създава се, ако не съществува), с редове <sha256sum на файл> <абсолютен път до файла>
#REPORTS_DIR — директория за докладите (създава се, ако не съществува)

#Докладът се записва в REPORTS_DIR с име <timestamp>.report (YYYY-MM-DD-HH-MM-SS), групиран по статус:

#<status 1>:
#<път>
#...
#<status 2>:
#...

#Възможни статуси: new (не присъстват в регистъра), unchanged (хешът съвпада), modified (хешът не съвпада).

#След изпълнение на скрипта, файлът-регистър трябва да съдържа само обновена информация за обходените файлове.


if [[ $# < 1 ]]; then
    exit 1
fi
if [[ -z "$REGISTRY_FILE" ]]; then
    exit 1
fi
if [[ -z "$REPORTS_DIR" ]]; then
    exit 1
fi
if [[ ! -f $REGISTRY_FILE ]]; then
    touch $REGISTRY_FILE
fi
if [[ ! -d $REPORTS_DIR ]]; then
    mkdir $REPORTS_DIR
fi
TIMESTAMP=$(date +'%Y-%m-%d-%H-%M-%S')
for MY_PATH in "${@}"; do
    ABS_PATH=$(realpath $MY_PATH)
    while read $FILE; do
        SHA=$(sha256sum $FILE)
        if grep -F "$FILE" $REGISTRY_FILE; then
            SHA_IN_FILE=$(grep -F "$FILE" $REGISTRY_FILE | awk '{ print $1 }')
            if [[ $SHA -eq $SHA_IN_FILE ]]; then
                echo "$FILE" >> unchanged.tmp
            else
               sed -i "s|$SHA_IN_FILE $FILE|$SHA $FILE|" $REGISTRY_FILE
               echo "$FILE" >> modified.tmp
            fi
        else
            echo "$SHA $FILE" >> $REGISTRY_FILE
            echo "$FILE" >> new.tmp
        fi
    done < <(find $ABS_PATH -type f )
done
echo "new:\n" >> "$REPORTS_DIR/$TIMESTAMP.report"
cat "${new.tmp}" >> "$REPORTS_DIR/$TIMESTAMP.report"
echo "unchanged:\n" >> "$REPORTS_DIR/$TIMESTAMP.report"
cat "${unchanged.tmp}" >> "$REPORTS_DIR/$TIMESTAMP.report"
echo "modified:\n" >> "$REPORTS_DIR/$TIMESTAMP.report"
cat "${modified.tmp}" >> "$REPORTS_DIR/$TIMESTAMP.report"
