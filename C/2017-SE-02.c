/*
Напишете програма на C, която да работи подобно на командата cat, реализирайки само следната функционалност:


общ вид на изпълнение: ./main [OPTION] [FILE]...
ако е подаден като първи параметър -n, то той да се третира като опция, което кара програмата ви да номерира (глобално) 
всеки изходен ред (започвайки от 1).
програмата извежда на STDOUT
ако няма подадени параметри (имена на файлове), програмата чете от STDIN
ако има подадени параметри – файлове, програмата последователно ги извежда
ако някой от параметрите е тире (-), програмата да го третира като специално име за STDIN


Примерно извикване:

$ cat a.txt
a1
a2
$ cat b.txt
b1
b2
b3
$ echo -e "s1\ns2" | ./main -n a.txt - b.txt
1 a1
2 a2
3 s1
4 s2
5 b1
6 b2
7 b3
*/
#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argv[1] == "-n")
    {
        int counter = 1;
        if (argc - 2 == 0)
        {
            char buff;
            ssize_t readSize;
            while ((readSize = read(0, &buff, sizeof(buff))) > 0)
            {
                if (write(1, &buff, sizeof(buff)) == -1) err(1, "cant write");
                if (buff == '\n')
                {
                    counter++;
                    if (write(1, &counter, sizeof(counter)) == -1) err(1, "cant write");
                    char space = ' ';
                    if (write(1, &space, 1) == -1) err(1, "haha");
                }
            }
            if (readSize == -1) err(1, "cant read");
        }
        for (int i = 0;i < argc - 2;i++)
        {
            char buff;
            ssize_t readSize;
            int fd = open()
            while ((readSize = read()))
        }
    }
}
