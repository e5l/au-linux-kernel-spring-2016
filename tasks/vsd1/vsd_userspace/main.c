/*
 * TODO parse command line arguments and call proper
 * VSD_IOCTL_* using C function ioctl (see man ioctl).
 */

#include "../vsd_driver/vsd_ioctl.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const char* VSD_PATH = "/dev/vsd";
void usage()
{
    printf("Usage: vsd_userspace size_set size | size_get \n");
}

int size_set(int size)
{
    if (size <= 0)
    {
        printf("Invalid size\n");
        return EXIT_FAILURE;
    }

    vsd_ioctl_set_size_arg_t new_size;
    new_size.size = size;

    int file = open("/dev/vsd", O_RDONLY);

    if (file == -1)
    {
        printf("VSD open failed\n");
        return EXIT_FAILURE;
    }

    int result = ioctl(file, VSD_IOCTL_SET_SIZE, &new_size);
    if (result == -1)
    {
        printf("Failed to set size\n");
        close(file);
        return EXIT_FAILURE;
    }

    close(file);
    return EXIT_SUCCESS;
}

int size_get()
{
    vsd_ioctl_get_size_arg_t size;
    int file = open("/dev/vsd", O_RDONLY);

    if (file == -1)
    {
        printf("VSD open failed\n");
        return EXIT_FAILURE;
    }

    int result = ioctl(file, VSD_IOCTL_GET_SIZE, &size);
    if (result == -1)
    {
        printf("Failed to get size\n");
        close(file);

        return EXIT_FAILURE;
    }

    printf("%lu\n", size.size);
    close(file);

    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        usage();
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "size_set") == 0)
    {
        if (argc < 3)
        {
            printf("Size unknown\n");
            return EXIT_FAILURE;
        }

        return size_set(atoi(argv[2]));
    }
    else if (strcmp(argv[1], "size_get") == 0)
    {
        return size_get();
    }

    usage();
    return EXIT_FAILURE;
}
