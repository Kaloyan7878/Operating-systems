/*
Напишете програма на C, която да работи подобно на командата cat, реализирайки само следната функционалност:


програмата извежда на STDOUT
ако няма подадени параметри, програмата чете от STDIN
ако има подадени параметри – файлове, програмата последователно ги извежда
ако някой от параметрите започва с тире (-), програмата да го третира като специално име за STDIN


Примерно извикване:

$ ./main f - g

– извежда съдържанието на файла f, после STDIN, след това съдържанието на файла g.
*/
#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
void readFrom(int fd)
{

    ssize_t readSize;
    char buff[4096];
    while ((readSize = read(fd, &buff, sizeof(buff))) > 0)
    {
        if (write(1, &buff, readSize) == -1) err(1, "cant write");
    }
    if (readSize == -1) err(1, "cant read");
}
int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        readFrom(0);
        return 0;
    }
    for (int i = 1; i < argc; i++)
    {
        int fd;
        if (argv[i][0] == '-') fd = 0;
        else
        {
            fd = open(argv[i], O_RDONLY);
            if (fd == -1) err(1, "cant open");
        }
        readFrom(fd);
        if (fd != 0) close(fd);
    }
    return 0;
}
