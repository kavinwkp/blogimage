#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {

    // 1. 判断管道文件是否存在
    int ret = access("fifo1", F_OK);
    if (ret == -1) {
        printf("make fifo\n");
        // 2. 创建管道文件
        int ret = mkfifo("fifo1", 0664);
        if (ret == -1) {
            perror("mkfifo");
            exit(0);
        }
    }

    // 3. 打开管道文件
    int fd = open("fifo1", O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(0);
    }
    for (int i = 0; i < 10; i++) {
        char buf[1024];
        sprintf(buf, "hello, %d\n", i);
        printf("data: %d\n", i);
        write(fd, buf, strlen(buf));
        sleep(1);
    }
    close(fd);
    
    return 0;
}