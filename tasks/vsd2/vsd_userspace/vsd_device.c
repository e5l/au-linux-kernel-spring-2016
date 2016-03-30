#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdlib.h>

#include "../vsd_driver/vsd_ioctl.h"
#include "vsd_device.h"

static int device;

int vsd_init()
{
    device = open("/dev/vsd", O_RDWR);
    return (device == -1) ? EXIT_FAILURE : EXIT_SUCCESS;
}

int vsd_deinit()
{
    return close(device);
}

int vsd_get_size(size_t *out_size)
{
    vsd_ioctl_get_size_arg_t size;

    if (ioctl(device, VSD_IOCTL_GET_SIZE, &size) == -1)
        return EXIT_FAILURE;

    *out_size = size.size;
    return EXIT_SUCCESS;
}

int vsd_set_size(size_t size)
{
    vsd_ioctl_set_size_arg_t new_size;
    new_size.size = size;

    if (ioctl(device, VSD_IOCTL_SET_SIZE, &new_size) == -1)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

ssize_t vsd_read(char* dst, off_t offset, size_t size)
{
    if (lseek(device, offset, SEEK_SET) < 0)
        return -1;

    return read(device, dst, size);
}

ssize_t vsd_write(const char* src, off_t offset, size_t size)
{
    if (lseek(device, offset, SEEK_SET) < 0)
        return -1;

    return write(device, src, size);
}

void* vsd_mmap(size_t offset)
{
    size_t len;
    vsd_get_size(&len);
    return mmap(0, len - offset, PROT_READ | PROT_WRITE, MAP_SHARED, device, offset);
}

int vsd_munmap(void* addr, size_t offset)
{
    size_t len;
    vsd_get_size(&len);
    return munmap(addr, len - offset);
}
