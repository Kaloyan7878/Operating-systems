/*
Напишете програма на C, която приема шест задължителни позиционни параметъра – имена на файлове.

Примерно извикване:

$ ./main affix postfix prefix infix suffix crucifixus

Всички файлове започват с хедър с фиксирана дължина от 16 байта. Пети и шести (спрямо ℤ+) байт от
хедъра дефинират uint16_t число count, което описва броя на елементите във файла. Файловете affix и infix се състоят
от елементи от тип uint16_t, файловете prefix и crucifixus – от елементи от тип uint8_t, postfix – от uint32_t, а suffix – от uint64_t.

Интервал наричаме наредена двойка числa, която дефинира номер (спрямо ℤ) на начален елемент и брой елементи от даден файл.
Комплект наричаме наредена четворка от интервали, които позиционно се отнасят спрямо файловете {post,pre,in,suf}fix.

Елементите на файла affix дефинират серия от комплекти, на чиято база програмата трябва да генерира изходния файл crucifixus.
*/
#include <err.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
typedef struct Pair {
    uint16_t start;
    uint16_t count;
} Pair;
int main(int argc, char* argv[])
{
    if (argc != 7) errx(1, "bad args");
    int affix = open(argv[1], O_RDONLY);
    int postfix = open(argv[2], O_RDONLY);
    int prefix = open(argv[3], O_RDONLY);
    int infix = open(argv[4], O_RDONLY);
    int suffix = open(argv[5], O_RDONLY);
    int crucifix = open(argv[6], O_WRONLY | O_TRUNC | O_CREAT, 0666);
    //proverki

    uint16_t count;
    lseek(affix, 4, SEEK_SET);
    if (read(affix, &count, sizeof(count)) == -1) err(1, "cant read count");
    lseek(affix, 16, SEEK_SET);
    uint16_t valid[4];
    lseek(postfix, 4, SEEK_SET);
    if (read(postfix, &valid[0], sizeof(valid[0])) == -1) err(1, "cant read count");
    lseek(prefix, 4, SEEK_SET);
    if (read(prefix, &valid[1], sizeof(valid[0])) == -1) err(1, "cant read count");
    lseek(infix, 4, SEEK_SET);
    if (read(infix, &valid[2], sizeof(valid[0])) == -1) err(1, "cant read count");
    lseek(suffix, 4, SEEK_SET);
    if (read(suffix, &valid[3], sizeof(valid[0])) == -1) err(1, "cant read count");
    for (int i = 0; i < count / 2; i++)
    {
        Pair pair;
        if (read(affix, &pair, sizeof(pair)) == -1) err(1, "cant read");
        if (i % 4 == 0)
        {
            if (pair.start * 4 > valid[0] * 4) errx(1, "error");
            if (pair.start * 4 + pair.count * 4 > valid[0] * 4) errx(1, "error");
            lseek(postfix, 16 + pair.start * 4, SEEK_SET);
            for (int j = 0; j < pair.count; j++)
            {
                uint32_t elem;
                if (read(postfix, &elem, sizeof(elem)) == -1) err(1, "cant read");
                if (write(crucifix, &elem, sizeof(elem)) == -1) err(1, "cant write");
            }
        }
        if (i % 4 == 1)
        {
            if (pair.start > valid[1]) errx(1, "error");
            if (pair.start + pair.count > valid[1]) errx(1, "error");
            lseek(prefix, 16 + pair.start, SEEK_SET);
            for (int j = 0; j < pair.count; j++)
            {
                uint8_t elem;
                if (read(prefix, &elem, sizeof(elem)) == -1) err(1, "cant read");
                if (write(crucifix, &elem, sizeof(elem)) == -1) err(1, "cant write");
            }
        }
        if (i % 4 == 2)
        {
            if (pair.start * 2 > valid[2] * 2) errx(1, "error");
            if (pair.start * 2 + pair.count * 2 > valid[2] * 2) errx(1, "error");
            lseek(infix, 16 + pair.start * 2, SEEK_SET);
            for (int j = 0; j < pair.count; j++)
            {
                uint16_t elem;
                if (read(infix, &elem, sizeof(elem)) == -1) err(1, "cant read");
                if (write(crucifix, &elem, sizeof(elem)) == -1) err(1, "cant write");
            }
        }
        if (i % 4 == 3)
        {
            if (pair.start * 8 > valid[3] * 8) errx(1, "error");
            if (pair.start * 8 + pair.count * 8 > valid[3] * 8) errx(1, "error");
            lseek(suffix, 16 + pair.start * 8, SEEK_SET);
            for (int j = 0; j < pair.count; j++)
            {
                uint64_t elem;
                if (read(suffix, &elem, sizeof(elem)) == -1) err(1, "cant read");
                if (write(crucifix, &elem, sizeof(elem)) == -1) err(1, "cant write");
            }
        }
    }
    close(affix);
    close(postfix);
    close(prefix);
    close(infix);
    close(suffix);
    close(crucifix);
    return 0;
}
