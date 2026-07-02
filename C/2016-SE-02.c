#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>
typedef struct Pair {
    uint32_t startOffset;
    uint32_t count;
} Pair;

int main(int argc, char* argv[])
{
    if (argc != 4) err(1, "bad args!");
    int f1 = open(argv[1], O_RDONLY);
    if (f1 == -1) err(1, "f1");
    struct stat st;
    if (fstat(f1, &st) == -1) err(1, "fstat");
    if (st.st_size % sizeof(Pair) != 0) errx(1, "bad f1");

    int f2 = open(argv[2], O_RDONLY);
    if (f2 == -1) err(1, "f2");
    if (fstat(f2, &st) == -1) err(1, "fstat");
    if (st.st_size % sizeof(uint32_t)) errx(1, "bad f2");

    int f3 = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (f3 == -1) err(1, "f3");

    Pair pair;
    ssize_t readSize;
    while ((readSize = read(f1, &pair, sizeof(pair))) > 0)
    {
        if (lseek(f2, pair.startOffset, SEEK_SET) == -1) err(1, "lseek");
        for (uint32_t i = 0; i < pair.count; i++)
        {
            uint32_t num;
            if (read(f2, &num, sizeof(num)) == -1) err(1, "f2");
            if (write(f3, &num, sizeof(num)) == -1) err(1, "f3");
        }
    }
    if (readSize == -1) err(1, "cant read");
    close(f1);
    close(f2);
    close(f3);
    return 0;
}
