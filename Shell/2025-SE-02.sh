#!/bin/bash
#Напишете скрипт manage.sh, който управлява сървиси. Скриптът очаква environment променливата SVC_DIR, съдържаща директория
#с описания на сървисите. Всеки файл в SVC_DIR описва сървис във вида:

#name: <име на сървис>
#pidfile: <име на pid файл>
#outfile: <име на файл>
#comm: <shell команда>

#manage.sh трябва да поддържа извиквания:

#./manage.sh start <име на сървис> — стартира сървиса (процес на команда comm във фонов режим), записва PID в pidfile, 
#пренасочва stdout/stderr в outfile; ако вече работи — не прави нищо.
#./manage.sh stop <име на сървис> — убива процеса със SIGTERM.
#./manage.sh running — извежда имената на всички сървиси, чиито процеси още работят, сортирани лексикографски.
#./manage.sh cleanup — изтрива PID-файловете на сървисите, чиито процеси не съществуват.


if [[ -z "$SVC_DIR" ]]; then
    exit 1
fi
if [[ ! -d "$SVC_DIR" ]]; then
    exit 1
fi
if [[ "$1" == "start" ]]; then
    FILE=$(find "$SVC_DIR" -type f -exec grep -l "name: $2" {} \;)
    comm=$(grep "^comm: " $FILE | sed "s/^comm: //")
    PID_FILE=$(grep "^pidfile: " $FILE | sed "s|^pidfile: ||")
    if [[ -f "$PID_FILE" ]]; then
        pid=$(cat "$PID_FILE")
        if kill -0 $pid; then
            exit 2
        fi
    fi
    OUT_FILE=$(grep "^outfile: " $FILE | sed "s|^outfile: ||")
    bash -c "$comm" > "$OUT_FILE" 2>&1 &
    echo "$!" >> "$PID_FILE"
fi
if [[ "$1" == "stop" ]]; then
    FILE=$(find "$SVC_DIR" -type f -exec grep -l "name: $2" {} \;)
    PID_FILE=$(grep "^pidfile: " $FILE | sed "s|^pidfile: ||")
    if [[ -z "$(cat "$PID_FILE")" ]]; then
        exit 3
    fi
    pid=$(cat $PID_FILE)
    kill -SIGTERM $pid
    sed -i "s/$pid//" "$PID_FILE"
fi
if [[ "$1" == "running" ]]; then
    while read FILE; do    
        PID_FILE=$(grep "^pidfile: " $FILE | sed "s|^pidfile: ||")
        if [[ -n "$(cat $PID_FILE)" ]]; then
            NAME=$(grep "^name: " $FILE | sed "s|^name: ||")
            echo "$NAME" >> /tmp/tmp.txt
        fi
    done < <(find $SVC_DIR -type f)
    sort /tmp/tmp.txt
    rm /tmp/tmp.txt
fi
if [[ "$1" == "cleanup" ]]; then
    while read FILE; do
        PID_FILE=$(grep "^pidfile: " $FILE | sed "s|^pidfile: ||")
        pid=$(cat "$PID_FILE")
        if [[ -n "$pid" ]]; then
            if ! kill -0 $pid; then
                rm $PID_FILE
            fi
        fi
    done < <(find $SVC_DIR -type f)
fi
