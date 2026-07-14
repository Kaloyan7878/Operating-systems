/*
Напишете програма-наблюдател P, която изпълнява друга програма Q и я рестартира, когато Q завърши изпълнението си.
На командния ред на P се подават следните параметри:


праг за продължителност в секунди – едноцифрено число от 1 до 9
Q
незадължителни параметри на Q


P работи по следния алгоритъм:


стартира Q с подадените параметри
изчаква я да завърши изпълнението си
записва в текстов файл run.log един ред с три полета - цели числа (разделени с интервал):

момент на стартиране на Q (Unix time)
момент на завършване на Q (Unix time)
код за грешка, с който Q е завършила (exit code)



проверява дали е изпълнено условието за спиране и ако не е, преминава отново към стартирането на Q


Условие за спиране: Ако наблюдателят P установи, че при две последователни изпълнения на Q са били изпълнени и двете условия:


кодът за грешка на Q е бил различен от 0;
разликата между момента на завършване и момента на стартиране на Q е била по-малка от подадения като първи параметър на P праг;


то P спира цикъла от изпълняване на Q и сам завършва изпълнението си.

Текущото време във формат Unix time можете да вземете с извикване на системната функция time() с параметър NULL. 
Ако изпълнената програма е била прекъсната от подаден сигнал, това се приема за завършване с код за грешка 129.
*/
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
