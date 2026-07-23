#!/bin/bash
#Задачата ви е да напишете скрипт benchmark.sh, който измерва средното време за изпълнение на дадена команда.
#Първият аргумент на скрипта е число (време за провеждане на експеримента, в секунди), а останалите аргументи са
#измерваната команда и нейните аргументи.

#Скриптът трябва да изпълнява подадената команда многократно, докато изтече подаденото време. Когато това се случи, 
#скриптът трябва да изчака последното извикване на командата да приключи и да изведе съобщение, описващо броя направени извиквания,
#общото и средното време за изпълнение.

$ ./benchmark.sh 60 convert image.jpg result.png
Ran the command 'convert image.jpg result.png' 8 times for 63 seconds.
Average runtime: 7.88 seconds.

$ ./benchmark.sh 10 sleep 1.5
Ran the command 'sleep 1.5' 7 times for 10.56 seconds.
Average runtime: 1.51 seconds.

Забележки: времената се извеждат в секунди, с точност два знака след запетайката; приемете, че времето на изпълнение на частите от скрипта извън подадената команда е пренебрежимо малко.
TIME=$1
TIME_RUN=0
HOW_MANY=0
while (( $(echo "$TIME_RUN <= $TIME" | bc) )); do
    START_TIME=$(date +%s.%N)
    "${@:2}" > /dev/null 2>&1
    END_TIME=$(date +%s.%N)
    CURRENT_RUN=$(echo "$END_TIME - $START_TIME" | bc)
    TIME_RUN=$(echo "$TIME_RUN + $CURRENT_RUN" | bc)
    (( HOW_MANY++ ))
done
TIME_RUN=$(echo "scale=2; $TIME_RUN / 1" | bc)
AVERAGE_TIME=$(echo "scale=2; $TIME_RUN / $HOW_MANY" | bc)
echo "Ran the command ${@:2} $HOW_MANY times for $TIME_RUN seconds."
echo "Average runtime:  $AVERAGE_TIME seconds."
