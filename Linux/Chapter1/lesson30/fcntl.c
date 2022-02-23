
/*
#include <unistd.h>
#include <fcntl.h>
int fcntl(int fd, int cmd, ... );
    + fd: 表示需要操作的文件描述符
    + cmd: 表示对文件描述符进行何种操作
        - F_DUPFD: 复制文件描述符，复制fd，结果返回
        - F_GETFL: 获取指定文件描述符文件状态flag
            flag和通过open函数传递的flag是一个东西
        - F_SETFL: 设置文件描述符文件状态flag
            必选项: O_RDONLY, O_WRONLY, O_RDWR
            可选项: O_APPEND, O_NONBLOCK（设置成非阻塞）
int ret = fcntl(fd, F_DUPFD);

*/

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main() {

    // int fd = open("1.txt", O_RDONLY);
    // int ret = fcntl(fd, F_DUPFD);

    int fd = open("1.txt", O_RDWR);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    int flag = fcntl(fd, F_GETFD);  // 原先的文件描述符
    flag |= O_APPEND;

    int ret = fcntl(fd, F_SETFL, flag);
    if (ret == -1) {
        perror("fcntl");
        return -1;
    }

    char *str = "hello";

    write(fd, str, strlen(str));

    close(fd);

    return 0;
}