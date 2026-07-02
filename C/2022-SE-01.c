#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
typedef struct CompElems
{
    uint16_t type;
    uint16_t reserved[3];
    uint32_t offset1;
    uint32_t offset2;
} CompElems;
int main(int argc, char* argv[])
{
    if (argc != 3) errx(1, "bad args");
    int dataFd = open(argv[1], O_RDWR);
    int comparatorFd = open(argv[2], O_RDONLY);
    if (dataFd == -1 || comparatorFd == -1) err(1, "cant open");
    uint32_t magicData;
    if (read(dataFd, &magicData, sizeof(magicData)) == -1) err(1, "cant read");
    if (magicData != 0x21796F4A) errx(1, "bad args");
    uint32_t magic1Comparator;
    uint16_t magic2Comparator;
    if (read(comparatorFd, &magic1Comparator, sizeof(magic1Comparator)) == -1) err(1, "cant read");
    if (read(comparatorFd, &magic2Comparator, sizeof(magic2Comparator)) == -1) err(1, "cant read");
    if (magic1Comparator != 0xAFBC7A37 || magic2Comparator != 0x1C27) errx(1, "bad args");
    uint32_t countData;
    if (read(dataFd, &countData, sizeof(countData)) == -1) err(1, "cant read");
    uint16_t reserved;
    if (read(comparatorFd, &reserved, sizeof(reserved)) == -1) err(1, "cant read");
    uint64_t countComparator;
    if (read(comparatorFd, &countComparator, sizeof(countComparator)) == -1) err(1, "cant read");
    for (uint64_t i = 0; i < countComparator; i++)
    {
        CompElems compElems;
        printf("%zu\n", sizeof(compElems));
        if (read(comparatorFd, &compElems, sizeof(compElems)) == -1) err(1, "cant read");
        if (compElems.offset1 >= countData || compElems.offset2 >= countData || compElems.offset1 == compElems.offset2) continue;
        lseek(dataFd, 8 + compElems.offset1 * 8, SEEK_SET);
        uint64_t num1;
        if (read(dataFd, &num1, sizeof(num1)) == -1) err(1, "cant read");
        lseek(dataFd, 8 + compElems.offset2 * 8, SEEK_SET);
        uint64_t num2;
        if (read(dataFd, &num2, sizeof(num2)) == -1) err(1, "cant read");
        if ((compElems.type == 0 && num1 > num2) || (compElems.type == 1 && num1 < num2))
        {
            lseek(dataFd, 8 + compElems.offset1 * 8, SEEK_SET);
            if (write(dataFd, &num2, sizeof(num2)) == -1) err(1, "cant write");
            lseek(dataFd, 8 + compElems.offset2 * 8, SEEK_SET);
            if (write(dataFd, &num1, sizeof(num1)) == -1) err(1, "cant write");
        }
    }
    close(dataFd);
    close(comparatorFd);
    return 0;
}
