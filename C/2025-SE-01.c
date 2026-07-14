/*
Дадени са множество файлове, всеки от които съдържа последователност от филмовите реплики на конкретна роля в сценарий,
представени в специфичен формат.

Напишете програма на C, която декодира подадените (до 20) като аргументи файлове и изписва всички реплики като текст на екрана, сортирани по време.

Всеки файл се състои от елементи от вида: идентификатор (64-битово число без знак), дължина на текста (8-битово число без знак N),
N на брой байта текст.

Първият елемент във всеки файл е хедър с идентификатор 133742, чийто текст е името на ролята. Останалите елементи са реплики -
идентификаторите им са сортирани възходящо времеви моменти.

Изходен формат:

име на роля x: реплика foo
име на роля y: реплика bar

глобално сортиран по времеви момент.
*/
#include <err.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
typedef struct Movieline
{
    uint64_t id;
    char role_name[256];
    char text[256];
} Movieline;
int main(int argc, char* argv[])
{
    if (argc >= 21) errx(1, "bad args");
    Movieline movielines[argc];
    char role_names[20][256];
    bool active[argc];
    int fds[argc];
    for (int i = 1; i < argc; i++)
    {
        fds[i] = open(argv[i], O_WRONLY);
        if (fds[i] == -1) err(1, "cant open");
        uint64_t h_id;
        uint8_t h_size;
        if (read(fds[i], &h_id, sizeof(h_id)) == -1) err(1, "cant read");
        if (read(fds[i], &h_size, sizeof(h_size)) == -1) err(1, "cant read");
        if(read(fds[i], role_names[i], h_size) == -1) err(1, "cant read");
        role_names[i][h_size] = '\0';
        uint8_t text_size;
        if (read(fds[i], &movielines[i].id, sizeof(movielines[i].id)) != -1)
        {
            if (read(fds[i], &text_size, sizeof(text_size)) == -1) err(1, "cant read");
            if (read(fds[i], movielines[i].text, text_size) == -1) err(1, "cant read");
            movielines[i].text[text_size] = '\0';
            strcpy(movielines[i].role_name, role_names[i]);
            active[i] = true;
        }
        else
        {
            active[i] = false;
            close(fds[i]);
        }
    }
    while (true)
    {
        int best_idx = -1;
        uint64_t min_id = UINT64_MAX;
        for (int i = 1; i < argc; i++)
        {
            if (active[i] && movielines[i].id < min_id)
            {
                min_id = movielines[i].id;
                best_idx = i;
            }
        }
        if (best_idx == -1) break;
        printf("%s: %s\n", movielines[best_idx].role_name, movielines[best_idx].text);
        uint8_t next_size;
        if (read(fds[best_idx], &movielines[best_idx].id, 8) != -1)
        {
            if (read(fds[best_idx], &next_size, sizeof(next_size)) == -1) err(1, "cant read");
            if (read(fds[best_idx], movielines[best_idx].text, next_size) == -1) err(1, "cant read");
            movielines[best_idx].text[next_size] = '\0';           
        }
        else
        {
            active[best_idx] = false;
            close(fds[best_idx]);
        }
    }
    return 0;
}
