#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    if (argc < 3) errx(1, "bad args");
    int fd = open("run.log", O_CREAT | O_WRONLY | O_APPEND, 0666);
    int barrier = atoi(argv[1]);
    int fail_count = 0;
    while (1)
    {
        time_t start = time(NULL);
        pid_t pid = fork();
        if (pid == -1) err(1, "fork");
        if (pid == 0)
        {
            execvp(argv[2], &argv[2]);
        }
        int status;
        if (wait(&status) == -1) err(1, "wait");
        time_t end = time(NULL);
        int exitCode;
        if (WIFEXITED(status))
        {
            exitCode = WEXITSTATUS(status); 
        }
        else
        {
            exitCode = 129;
        }
        
        char text[128];
        snprintf(text, sizeof(text), "%ld %ld %d", start, end, exitCode);
        if (write(fd, text, strlen(text)) == -1) err(1, "cant write");

        if (exitCode != 0 && ((end - start) < barrier))
        {
            fail_count++;
        }
        else
        {
            fail_count = 0;
        }
        if (fail_count == 2) break;
    }
    close(fd);
    return 0;
}
