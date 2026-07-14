/*
Напишете програма на C, която реализира simple command prompt. Тя изпълнява в цикъл следната поредица действия:


Извежда промпт на стандартния изход.
Прочита име на команда.
Изпълнява без параметри прочетената команда.


Командите се търсят в директорията /bin. За край на програмата се смята въвеждането на exit.
*/
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
const char prompt[] = "$";
int main(void)
{
    while (true)
    {
        if (write(1, &prompt, strlen(prompt)) == -1) err(1, "cant write promp");
                
        char command[128];
        ssize_t readSize;
        if ((readSize = read(0, &command, sizeof(command))) == -1) err(1, "cant read");
        command[readSize - 1] = '\0';
        if (strcmp(command, "exit") == 0) break;
        pid_t pid = fork();
        if (pid < 0)
        {
            err(1, "fork");
        }
        char fullpath[256];
        snprintf(fullpath, sizeof(fullpath), "/bin/%s", command);
        if (pid == 0)
        {
            int result = execl(fullpath, command, NULL);
            if (result < 0) err(1, "exec");
        }
        else
        {
            int status;
            if (wait(&status) == -1) err(1, "wait");
            if (!WIFEXITED(status)) 
            {
                warnx("child was killed");
            }
        }
    }
    return 0;
}
