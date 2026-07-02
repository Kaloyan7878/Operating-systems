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
