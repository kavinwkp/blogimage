#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {

    int srcfd = open("e.txt", O_RDONLY);

    if (srcfd == -1) {
        perror("open");
        return -1;
    }

    int destfd = open("cpy.txt", O_WRONLY | O_CREAT, 0664);
    if (destfd == -1) {
        perror("open");
        return -1;
    }

    char buf[1024] = {0};

    int len = 0;

    if ((len = read(srcfd, buf, sizeof(buf))) > 0) {
        printf("read %d bytes\n", len);
        write(destfd, buf, len);
    }

    close(srcfd);
    close(destfd);
    return 0;
}