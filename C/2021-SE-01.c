/*
Инженерите от съседната лаборатория ползват специализиран хардуер и софтуер за прехвърляне на данни по радио, но за съжаление имат два проблема:


в радио частта: дълги поредици битове само 0 или само 1 чупят преноса;
в софтуерната част: софтуерът, който ползват, може да прехвърля само файлове с четен брой байтове дължина.


Помогнете на колегите си, като напишете програма на C, която решава тези проблеми, като подготвя файлове за прехвърляне.
Програмата трябва да приема два задължителни позиционни аргумента – имена на файлове.

Примерно извикване:

$ ./main input.bin output.bin

Програмата чете данни от input.bin и записва резултат след обработка в output.bin. Програмата трябва да работи като encoder,
който имплементира вариант на Manchester code, т.е.:


за всеки входен бит 1 извежда битовете 10, и
за всеки входен бит 0 извежда битовете 01


Например, следните 8 бита вход 1011 0110 == 0xB6 дават следните 16 бита изход 1001 1010 0110 1001 == 0x9A69.
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
    int fdIn = open(argv[1], O_RDONLY);
    int fdOut = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (fdIn == -1 || fdOut == -1) err(1,"cant open");
    uint8_t byte;
    ssize_t readSize;
    while ((readSize = read(fdIn, &byte, sizeof(byte))) > 0)
    {
        uint16_t newByte = 0;
        uint8_t mask = (1 << 7);
        uint16_t one = (1 << 15);
        uint16_t zero = (1 << 14);
        for (int i = 0; i < 8; i++)
        {
            if (byte & mask)
            {
                newByte = newByte | one;
            }
            else
            {
                newByte = newByte | zero;
            }
            one >>= 2;
            zero >>= 2;
            mask >>= 1;
        }
        uint8_t out[2];
        out[0] = (newByte >> 8) & 0xFF; // Старши байт (първите 4 бита)
        out[1] = newByte & 0xFF;        // Младши байт (вторите 4 бита)
        if (write(fdOut, out, 2) == -1) err(1, "cant write");
    }
    if (readSize == -1) err(1, "cant read");
    close(fdIn);
    close(fdOut);
    return 0;
}
