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

    ssize_t r_sz = read(fd, inbuf, size);
    if (r_sz != size) {
        perror("Failed to write character device");
        goto error;
    }

    bool pass = true;
    ret = 0;
    /* Verify the result of sorting */
    for (size_t i = 1; i < nelem; i++) {
        if (inbuf[i] < inbuf[i - 1]) {
            pass = false;
            break;
        }
    }

    if (pass)
        printf("Sorting success!\n");
    else
        printf("Sorting fail!\n");

error:
    free(inbuf);
    if (fd > 0)
        close(fd);
    return ret;
}
