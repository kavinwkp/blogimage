#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {

    struct stat statbuf;

    int ret = lstat("b.txt", &statbuf);

    if (ret == -1) {
        perror("open");
        return -1;
    }

    printf("size: %ld\n", statbuf.st_size);

    return 0;
}