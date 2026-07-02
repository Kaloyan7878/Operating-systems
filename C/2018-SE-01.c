#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
//nqma smisul da proverqvash mode na vsqka iteraciq!!!
int main(int argc, char* argv[])
{
    if (argc < 3) errx(1, "bad args");
    ssize_t readSize;
    char buff;
    char* mode = argv[1];
    char symbol = '\0';
    while ((readSize = read(0, &buff, sizeof(buff))) > 0)
    {
        if (strcmp(mode, "-d") == 0)
        {
            int found = 0;
            for (int i = 0; argv[2][i] != '\0'; i++)
            {
                if (buff == argv[2][i])
                {
                    found = 1;
                    break;
                }
            }
            if (!found && (write(1, &buff, sizeof(buff)) == -1))
            {
                err(1, "cant write");
            }
        }
        else if (strcmp(mode, "-s") == 0)
        {
            if (buff != symbol)
            {
                if (write(1, &buff, sizeof(buff)) == -1) err(1, "cant write");
                for (int i = 0; argv[2][i] != '\0'; i++)
                {
                    if (buff == argv[2][i])
                    {
                        symbol = buff;
                        break;
                    }
                }
            }

        }
        else
        {
            int checked = 0;
            if (!checked && (strlen(argv[1]) != strlen(argv[2])))
            {
                errx(1, "bad strings");
                checked = 1;
            }
            int found = 0;
            for (int i = 0; argv[1][i] != '\0'; i++)
            {
                if (buff == argv[1][i])
                {
                    if (write(1, &argv[2][i], sizeof(argv[2][i])) == -1) err(1, "cant write");
                    found = 1;
                    break;                    
                }
            }
            if (!found && (write(1, &buff, sizeof(buff)) == -1)) err(1, "cant write"); 
        }
    }
    if (readSize == -1) err(1, "cant read");
}
