#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    // 1. 打开管道文件
    int fd = open("fifo1", O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(0);
    }
    while (1) {
        char buf[1024] = {0};
        int len = read(fd, buf, sizeof(buf));
        if (len == 0) {
            printf("write end close...\n");
            break;
        }
        printf("recv buf: %s\n", buf);
    }
    close(fd);
    
    return 0;
}