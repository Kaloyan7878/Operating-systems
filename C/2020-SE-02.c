/*
При изграждане на система за пренасяне на сериен асинхронен сигнал върху радиопреносна мрежа се оказало,
че големи поредици от битове само нули или само единици смущават сигнала, поради нестабилно ниво. Инженерите решили проблема, като:


в моментите, в които няма сигнал от серийния порт, вкарвали изкуствено байт 0x55 в потока;
реалните байтове 0x00, 0xFF, 0x55 и 0x7D се кодирали посредством XOR-ване с 0x20, като полученият байт се изпращал през потока,
предхождан от 0x7D, който играе ролята на escape character.


Разполагате със запис от такъв поток. Напишете програма на C, която приема два параметъра - имена на файлове.

Примерно извикване:

$ ./main input.lfld output.bin

Програмата трябва да обработва записа и да генерира output.bin, който да съдържа оригиналните данни.
Четенето на входните данни трябва да става посредством изпълнение на външна shell команда.
*/
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
