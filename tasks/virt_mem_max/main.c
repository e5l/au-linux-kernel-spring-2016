#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

int main()
{
    size_t size = -1;
    size_t result = 0;

    int fd = open("/dev/zero", O_RDONLY);

    while (size != 0)
    {
        int64_t i = (int64_t) mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (i != -1)
            result += size;

        size /= 2;
    }

    size = -1;

    printf("%zu\n", result);
    return 0;
}
