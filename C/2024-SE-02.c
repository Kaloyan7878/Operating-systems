/*
Напишете програма наблюдател, която няколкократно изпълнява други програми до тяхното успешно изпълнение.

Програмата получава между 1 и 10 аргумента – имена на наблюдавани програми:

./main <програма 1> <програма 2> ... <програма N>

Програмите се изпълняват паралелно:


при успешно завършване (нулев статус) – спира да се използва
при неуспешно завършване (ненулев статус) – изпълнява се наново
ако процес бъде убит преди да завърши (напр. краш), наблюдателят:

Прекратява всички други текущо изпълняващи се процеси (SIGTERM, после изчаква)
Завършва със статус, съответстващ на номера на убитата програма
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
int main(int argc, char* argv[])
{
    if (argc < 2 || argc > 11) errx(1, "bad args");
    pid_t pid[argc - 1];
    for (int i = 0; i < argc - 1; i++)
    {
        pid[i] = 0;
    }
    int remaining = argc - 1;
    while (remaining > 0)
    {
        for (int i = 0; i < argc - 1; i++)
        {   
            if (pid[i] == 0)
            {
                pid[i] = fork();       
                if (pid[i] == 0)
                {
                    execlp(argv[i + 1], argv[i + 1], NULL);
                    err(27, "exec");
                }
            }
        }
        int status;
        pid_t dead_pid;
        int ended_index = -1;
        if ((dead_pid = wait(&status)) == -1) err(1, "wait");
        for (int i = 0; i < argc - 1; i++) 
        {
            if (pid[i] == dead_pid) 
            {
                ended_index = i;
                break;
            }
        }
        if (!WIFEXITED(status))
        {
            for (int j = 0; j < argc - 1; j++)
            {
                if (pid[j] > 0 && j != ended_index)
                {
                    kill(pid[j], SIGTERM);
                    waitpid(pid[j], NULL, 0);
                }
            }
            return ended_index + 1;
        }
        else if (WEXITSTATUS(status) == 0)
        {
            pid[ended_index] = -1;
            remaining--;
        }
        else
        {
            pid[ended_index] = 0;
        }
    }
    return 0;
}
