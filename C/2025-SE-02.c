/*
Дадена ви е база от данни от два файла: индекс и стойности. Индексът съдържа записи с променлива дължина,
позиционно съответстващи на 32-битови стойности от файла със стойности:


байт 0, битове 0-6: дължина N на името на записа
байт 0, бит 7: тип на стойността – 0 цяло число със знак, 1 single-precision float
байтове 1..N: текст с името


Напишете програма, приемаща 3 аргумента (индекс файл, файл стойности, търсено име), която извежда стойността на
първия запис с това име: цяло число или число с 3 знака след запетаята при float. При липсващ запис или несъответствие в размерите – грешка.
*/
#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
int main(int argc, char* argv[])
{
    if (argc != 4) errx(1, "bad args");
    int fdIdx = open(argv[1], O_RDONLY);
    int fdVal = open(argv[2], O_RDONLY);
    if (fdIdx == -1 || fdVal == -1) err(1, "cant open");
    struct stat st;
    if (fstat(fdVal, &st) == -1) err(1, "stat");
    if (st.st_size % 4 != 0) errx(1, "invalid values");

    uint8_t byte;
    ssize_t readSize;
    int position = 0;
    bool found = false;
    while ((readSize = read(fdIdx, &byte, sizeof(byte))) > 0)
    {
        int length = byte & ~(1 << 7);
        int type = (byte >> 7) & 1;
        char name[UINT8_MAX + 1];
        if (read(fdIdx, name, length) == -1) err(1, "cant read"); 
        name[length] = '\0';
        if (strcmp(name, argv[3]) == 0)
        {
            if (lseek(fdVal, position * 4, SEEK_SET) == -1) err(1, "lseek");
            char buff[1024];
            if (type == 0)
            {
                uint32_t value;
                if (read(fdVal, &value, sizeof(value)) == -1) err(1, "cant read");
                snprintf(buff, sizeof(buff), "%d\n", value);
            }
            else
            {
                float value;
                if (read(fdVal, &value, sizeof(value)) == -1) err(1, "cant read");
                snprintf(buff, sizeof(buff), "%.3f\n", value);
            }
            if (write(1, buff, strlen(buff)) == -1) err(1, "cant write");
            found = true;
            break;
        }
        position++;
    }
    if (!found) errx(1, "value not found");
    close(fdIdx);
    close(fdVal);
    return 0;
}
