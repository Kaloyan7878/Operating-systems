/*
Двоичните файлове f1 и f2 съдържат 32 битови числа без знак (uint32_t). Файлът f1 съдържа n двойки числа, нека i-тата двойка е < xi, yi >. Напишете програма на C,
която извлича интервалите с начало xi и дължина yi от файла f2 и ги записва залепени в изходен файл f3.

Пример:
f1 съдържа 4 числа (2 двойки): 30000, 20, 19000, 10
програмата записва в f3 две поредици 32-битови числа, взети от f2, както следва:

най-напред числата на позиции 30000, 30001, ... 30019
след тях числата от позиции 19000, 19001, ... 19009

Забележка: С пълен брой точки ще се оценяват решения, които работят със скорост, пропорционална на размера на изходния файл f3.
*/
#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>
typedef struct Pair {
    uint32_t startOffset;
    uint32_t count;
} Pair;

int main(int argc, char* argv[])
{
    if (argc != 4) err(1, "bad args!");
    int f1 = open(argv[1], O_RDONLY);
    if (f1 == -1) err(1, "f1");
    struct stat st;
    if (fstat(f1, &st) == -1) err(1, "fstat");
    if (st.st_size % sizeof(Pair) != 0) errx(1, "bad f1");

    int f2 = open(argv[2], O_RDONLY);
    if (f2 == -1) err(1, "f2");
    if (fstat(f2, &st) == -1) err(1, "fstat");
    if (st.st_size % sizeof(uint32_t)) errx(1, "bad f2");

    int f3 = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (f3 == -1) err(1, "f3");

    Pair pair;
    ssize_t readSize;
    while ((readSize = read(f1, &pair, sizeof(pair))) > 0)
    {
        if (lseek(f2, pair.startOffset, SEEK_SET) == -1) err(1, "lseek");
        for (uint32_t i = 0; i < pair.count; i++)
        {
            uint32_t num;
            if (read(f2, &num, sizeof(num)) == -1) err(1, "f2");
            if (write(f3, &num, sizeof(num)) == -1) err(1, "f3");
        }
    }
    if (readSize == -1) err(1, "cant read");
    close(f1);
    close(f2);
    close(f3);
    return 0;
}
