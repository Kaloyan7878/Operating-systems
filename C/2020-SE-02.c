#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    if (argc != 3) errx(1, "bad args");
    
    int pfd[2];
    if (pipe(pfd) == -1) err(1, "cant pipe");
    pid_t pid = fork();
    if (pid == -1) err(1, "cant fork");
    if (pid == 0)
    {
        close(pfd[0]);
        dup2(pfd[1], 1);
        close(pfd[1]);
        execlp("cat", "cat", argv[1], NULL);
        err(1, "cant exec");
    }
    close(pfd[1]);
    int fdOut = open(argv[2], O_TRUNC | O_WRONLY | O_CREAT, 0666);
    if (fdOut == -1) err(1, "cant open");

    uint8_t byte;
    ssize_t readSize;
    while ((readSize = read(pfd[0], &byte, sizeof(byte))) > 0)
    {
        if (byte == 0x55) continue;
        if (byte == 0x7D)
        {
            uint8_t newByte;
            if (read(pfd[0], &newByte, sizeof(newByte)) == -1) err(1, "cant read");
            newByte ^= 0x20;
            if (write(fdOut, &newByte, sizeof(newByte)) == -1) err(1, "cant write");
        }
        else
        {
            if (write(fdOut, &byte, sizeof(byte)) == -1) err(1, "cant write");
        }
    }
    if (readSize == -1) err(1, "cant read");
    close(pfd[0]);
    close(fdOut);
    int status;
    if (wait(&status) == -1) err(1, "wait");
    if (!WIFEXITED(status)) err(1, "child was killed");
    return 0;
}
