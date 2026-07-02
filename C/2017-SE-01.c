#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
typedef struct Entry {
    uint16_t offset;
    uint8_t old;
    uint8_t new;
} Entry;
int main(int argc, char* argv[])
{
    if (argc != 4) errx(1, "bad args");
    int f1 = open(argv[1], O_RDONLY);
    if (f1 == -1) err(1, "f1");
    int f2 = open(argv[2], O_RDONLY);
    struct stat st;
    if (fstat(f1, &st) == -1) err(1, "stat");
    off_t f1Size = st.st_size;
    if (fstat(f2, &st) == -1) err(1, "stat");
    off_t f2Size = st.st_size;
    if (f1Size != f2Size) errx(1, "files not equal");

    int patch = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (patch == -1) err(1, "patch");
    int offset = 0;
    uint8_t byteFromF1;
    ssize_t readSize;
    while ((readSize = read(f1, &byteFromF1, sizeof(byteFromF1))) > 0)
    {
        uint8_t byteFromF2;
        if (read(f2, &byteFromF2, sizeof(byteFromF2)) == -1) err(1, "f2");
        if (byteFromF1 != byteFromF2)
        {
            Entry entry;
            entry.offset = offset;
            entry.old = byteFromF1;
            entry.new = byteFromF2;
            if (write(patch, &entry, sizeof(entry)) == -1) err(1, "patch");
        }
        offset++;
    }
    if (readSize == -1) err(1, "f1");
    close(f1);
    close(f2);
    close(patch);
    return 0;
}
