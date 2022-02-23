#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


int main() {

    int fd = open("1.txt", O_RDWR | O_CREAT, 0664);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    int fd1 = open("2.txt", O_RDWR | O_CREAT, 0664);

    printf("fd: %d, fd1: %d\n", fd, fd1);

    int fd2 = dup2(fd, fd1);
    if (fd2 == -1) {
        perror("dup2");
        return -1;
    }

    char *str = "hello, world";

    int ret = write(fd1, str, strlen(str));
    if (ret == -1) {
        perror("write");
        return -1;
    }

    printf("fd: %d, fd1: %d, fd2: %d\n", fd, fd1, fd2);

    close(fd);
    close(fd1);


    return 0;
}