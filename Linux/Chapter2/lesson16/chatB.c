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
    ret = access("fifo2", F_OK);
    if (ret == -1) {
        printf("make fifo\n");
        // 2. 创建管道文件
        int ret = mkfifo("fifo2", 0664);
        if (ret == -1) {
            perror("mkfifo");
            exit(0);
        }
    }

    // 3. 打开管道文件
    int fdr = open("fifo1", O_RDONLY);
    if (fdr == -1) {
        perror("open");
        exit(0);
    }
    printf("open fifo1, wait to read\n");

    int fdw = open("fifo2", O_WRONLY);
    if (fdw == -1) {
        perror("open");
        exit(0);
    }
    printf("open fifo2, wait to write\n");

    // 4. 循环读写数据
    char buf[128];
    while (1) {
        memset(buf, 0, 128);
        int ret = read(fdr, buf, 128);
        if (ret <= 0) {
            perror("read");
            break;
        }
        printf("buf: %s\n", buf);

        memset(buf, 0, 128);
        fgets(buf, 128, stdin);
        ret = write(fdw, buf, strlen(buf));
        if (ret == -1) {
            perror("write");
            exit(0);
        }
    }
    // 关闭文件描述符
    close(fdw);
    close(fdr);
    
    return 0;
}