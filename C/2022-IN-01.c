/*
Ваши колеги - асистенти по ОС имат нужда от демонстрационна програма на C, която да служи като пример за конкурентност
и синхронизация на процеси. Напишете такава програма, приемаща два задължителни позиционни параметъра – едноцифрени числа.

Примерно извикване:

./main N D

Общ алгоритъм на програмата:


началният (родителски) процес създава процес-наследник
N на брой пъти се изпълнява:

родителският процес извежда на stdout низа "DING "
процесът-наследник извежда на stdout низа "DONG"
родителският процес изчаква D секунди





Гарантирайте, че:


процесът-наследник винаги извежда "DONG" след като родителският процес е извел "DING "
родителският процес винаги започва изчакването след като процесът-наследник е извел "DONG"


Забележка: За изчакването погледнете sleep(3).
*/
#include <err.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

const char* DING = "DING ";
const char* DONG = "DONG \n";

void doParent(int fromChild, int toChild, int N, int D)
{
    for (int i = 0; i < N; i++)
    {
        uint8_t buff = 0;
        if (write(1, DING, strlen(DING)) == -1) err(1, "cant write");
        if (write(toChild, &buff, sizeof(buff)) == -1) err(1, "cant write");
        if (read(fromChild, &buff, sizeof(buff)) == -1) err(1, "cant read");
        sleep(D);
    }
}

void doChild(int fromParent, int toParent,int N)
{
    for (int i = 0; i < N; i++)
    {
        uint8_t buff = 0;
        if (read(fromParent, &buff, sizeof(buff)) == -1) err(1, "cant read");
        if (write(1, DONG, strlen(DONG)) == -1) err(1, "cant write");
        if (write(toParent, &buff, sizeof(buff)) == -1) err(1, "cant write");
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3) errx(1, "bad args");
    int N = atoi(argv[1]);
    int D = atoi(argv[2]);
    
    int parentToChild[2];
    if (pipe(parentToChild) == -1) err(1, "pipe");
    int childToParent[2];
    if (pipe(childToParent) == -1) err(1, "pipe");

    int pid = fork();
    if (pid == -1) err(1, "fork");
    if (pid == 0)
    {
        close(parentToChild[1]);
        close(childToParent[0]);

        int fromParent = parentToChild[0];
        int toParent = childToParent[1];
        doChild(fromParent, toParent, N);

        close(fromParent);
        close(toParent);
        exit(0);
    }

    close(parentToChild[0]);
    close(childToParent[1]);

    int fromChild = childToParent[0];
    int toChild = parentToChild[1];

    doParent(fromChild, toChild, N, D);

    close(fromChild);
    close(toChild);

    int status;
    if (wait(&status) == -1) err(1, "wait");
    if (!WIFEXITED(status)) warnx("child was killed");
    return 0;
}
