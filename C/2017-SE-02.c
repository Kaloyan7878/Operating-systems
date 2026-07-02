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
