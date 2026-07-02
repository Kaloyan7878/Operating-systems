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
