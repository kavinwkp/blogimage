// TCP通信的客户端

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {
    // 1. 创建套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        exit(-1);
    }

    // 2. 连接服务器
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, "10.37.62.58", &serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(9999);
    int ret = connect(fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (ret == -1) {
        perror("connect");
        exit(-1);
    }

    // 3. 通信
    char recvBuf[1024] = {0};
    int i = 0;
    while (1) {
        // char *data = "hello, I am client";
        sprintf(recvBuf, "data: %d\n", i++);
        write(fd, recvBuf, strlen(recvBuf) + 1);    // 带上字符串结束符

        int len = read(fd, recvBuf, sizeof(recvBuf));
        if (len == -1) {
            perror("read");
            exit(-1);
        }
        else if (len > 0) {
            printf("receive %s\n", recvBuf);
        }
        else if (len == 0) {
            // 服务器端断开连接
            printf("server close\n");
            break;
        }
        sleep(1);

    }

    // 关闭连接
    close(fd);

    return 0;
}