/*
Напишете програма на C, която приема един задължителен позиционен параметър - име на файл. Файлът се състои от не повече от 8 наредени тройки елементи:


име на файл – точно 8 байта, последният от които задължително е 0x00. Ако името е по-късо от 7 знака, излишните байтове са 0x00;
offset – uint32_t, който дава пореден номер на елемент (спрямо N0) във файла;
length – uint32_t, който дава брой елементи.


За всяка наредена тройка програмата трябва да пусне child процес, който да XOR-не (обработи с изключващо или) елементите (uint16_t)
от съответния файл един със друг, и да върне резултата на parent процеса, който от своя страна трябва да XOR-не всички
получени резултати и да изведе полученото число във формат:

result: 573B
*/
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Triple
{
    uint64_t name;
    uint32_t offset;
    uint32_t length;
} Triple;
int main(int argc, char* argv[])
{
    if (argc != 2) errx(1, "bad args");
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) err(1, "cant open");
    uint16_t xorSumAll = 0;
    Triple triple;
    ssize_t readSize;
    while ((readSize = read(fd, &triple, sizeof(triple))) > 0)
    {
        int pfd[2];
        if (pipe(pfd) < 0) err(1, "pipe");
        pid_t pid = fork();
        if (pid == -1) err(1, "fork");
        uint16_t xorSum = 0;
        if (pid == 0)
        {
            close(pfd[0]);
            char* fileName = (char*)&triple.name;
            int fdChild = open(fileName, O_RDONLY);
            lseek(fdChild, triple.offset * 2, SEEK_CUR);
            for (uint32_t i = 0; i < triple.length; i++)
            {
                uint16_t elem;
                if (read(fdChild, &elem, sizeof(elem)) == -1) err(1, "cant read");
                xorSum ^= elem;
            }
            if (write(pfd[1], &xorSum, sizeof(xorSum)) == -1) err(1, "cant write");
            close(pfd[1]);
            exit(0);
        }
        int status;
        if (wait(&status) == -1) err(1, "wait");
        close(pfd[1]);
        if (read(pfd[0], &xorSum, sizeof(xorSum)) == -1) err(1, "cant write");
        close(pfd[0]);
        xorSumAll ^= xorSum;
    }
    if (readSize == -1) err(1, "cant read");
    close(fd);
    char final_text[32];
    snprintf(final_text, sizeof(final_text), "result: %04X\n", xorSumAll);
    write(1, final_text, strlen(final_text));
    return 0;
}
