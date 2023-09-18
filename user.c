#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define KSORT_DEV "/dev/ksort"

int main()
{
    int ret = -1;
    int fd = -1;
    int *inbuf = NULL;

    fd = open(KSORT_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open character device");
        goto error;
    }

    size_t nelem = 1000;
    size_t size = nelem * sizeof(int);
    inbuf = malloc(size);
    if (!inbuf) {
        goto error;
    }
    for (size_t i = 0; i < nelem; i++)
        inbuf[i] = rand() % nelem;

    ssize_t w_sz = write(fd, inbuf, size);
    if (w_sz != size) {
        perror("Failed to write character device");
        ret = w_sz;
        goto error;
    }

    ret = 0;

error:
    free(inbuf);
    if (fd > 0)
        close(fd);
    return ret;
}
