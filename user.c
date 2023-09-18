#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define KSORT_DEV "/dev/ksort"

int main()
{
    size_t nelem = 1000;
    size_t size = nelem * sizeof(int);
    int *buf = malloc(size);

    int fd = open(KSORT_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }

    ssize_t sz = write(fd, buf, 0);

    close(fd);
    return 0;
}
