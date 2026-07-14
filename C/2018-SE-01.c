/*
Напишете програма на C, която да работи подобно на командата tr, реализирайки само следната функционалност:


програмата чете от стандартния вход и пише на стандартния изход
общ вид на изпълнение: ./main [OPTION] SET1 [SET2]
OPTION би могъл да бъде или -d, или -s, или да липсва
SET1 и SET2 са низове, в които знаците нямат специално значение. SET2, ако е необходим, трябва да е същата дължина като SET1
при -d, програмата трие от входа всяко срещане на знак μ от SET1; SET2 не е необходим
при -s, програмата заменя от входа всяка поредица от повторения знак μ от SET1 с еднократен знак μ; SET2 не е необходим
в останалите случаи програмата заменя от входа всеки знак μ от SET1 със съответстващия му позиционно знак ν от SET2.


Примерно извикване:

$ echo asdf | ./main -d 'd123' | ./main 'sm' 'fa' | ./main -s 'f'
af
*/
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
