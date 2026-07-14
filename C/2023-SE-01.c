/*
Имате файл, съдържащ откъс от поток от съобщения, който може би е повреден на места.

Трябва да напишете програма, която по даден такъв файл, генерира нов файл, съдържащ точно всички последователности от байтове от оригиналния,
които са валидни съобщения.

Примерно извикване: ./main stream.bin messages.bin, където stream.bin е име на съществуващ файл, а messages.bin трябва да се създаде (или презапише).

Валидно съобщение с дължина N байта има вида:


байт 1 — 0x55 – указва начало на съобщение
байт 2 — числото N – представено като 8-битово число без знак
байтове от 3 до (N−1) — произволни данни (съдържанието)
байт N — checksum на байтове от 1 до (N−1)


Checksum на поредица от байтове е резултатът от прилагане на операцията „побитово изключващо или“ (xor) върху тях.

Вариант Б — Зад. 131 (раздел 2.2)

Напишете програма на C, която приема като аргумент име на директория и генерира хешове на файловете в нея, работейки паралелно.

При изпълнение върху директория my-dir, програмата:


За всеки файл в директорията и нейните поддиректории (напр. my-dir/foo/my-file), трябва да създаде файл my-dir/foo/my-file.hash,
съдържащ хеш-сумата на my-file
Може да извиква външните команди find (за откриване на файлове) и md5sum (за пресмятане на хеш-сума)
Не трябва да обработва файлове, чиито имена вече завършват на .hash
Трябва да позволява md5sum-процесите да работят паралелно един спрямо друг
*/
#include <err.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
int main(int argc, char* argv[])
{
    if (argc != 3) errx(1, "bad args");
    int streamFd = open(argv[1], O_RDONLY);
    int messagesFd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (streamFd == -1 || messagesFd == -1) err(1, "cant open");
    ssize_t readSize;
    uint8_t byte;
    while ((readSize = read(streamFd, &byte, sizeof(byte))) > 0)
    {
        if (byte == 0x55)
        {
            int pos = lseek(streamFd, 0 ,SEEK_CUR);
            uint8_t N;
            if (read(streamFd, &N, sizeof(N)) == -1) err(1, "cant read");
            uint8_t message[256];
            message[0] = 0x55;
            message[1] = N;
            uint8_t xorSum = 0;
            for (int i = 0; i < N - 3; i++)
            {
                if (read(streamFd, &byte, sizeof(byte)) == -1) err(1, "cant read");
                xorSum ^= byte;
            }
            if (read(streamFd, &byte, sizeof(byte)) == -1) err(1, "cant read");
            if (xorSum == byte)
            {
                if (write(messagesFd, message, N) == -1) err(1, "cant write");
            }
            else
            {
                lseek(streamFd, pos, SEEK_SET);
            }
        }
    }
    close(streamFd);
    close(messagesFd);
    return 0;
}
