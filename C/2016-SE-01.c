#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
int main(int argc, char* argv[])
{
    if (argc != 2) errx(1, "bad args");
    int pfd[2];
    if (pipe(pfd) < 0)
    {
        err(1, "couldnt create pipe");
    }
    pid_t pid1 = fork();
    if (pid1 < 0) err(1, "cant fork");
    if (pid1 == 0)
    {
        //cat
        dup2(pfd[1], 1);
        close(pfd[1]);
        close(pfd[0]);
        execlp("cat", "cat", argv[1], NULL);
    }
    
    pid_t pid2 = fork();
    if (pid2 < 0) err(1, "cant fork");
    if (pid2 == 0)
    {
        //sort
        dup2(pfd[0], 0);
        close(pfd[1]);
        close(pfd[0]);
        execlp("sort", "sort", NULL);
    }
    close(pfd[0]);
    close(pfd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    return 0;
}
