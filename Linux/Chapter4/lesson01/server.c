// TCP通信的服务器端

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {
    // 1. 创建用于监听的套接字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1) {
        perror("socket");
        exit(-1);
    }

    // 2. 绑定
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    // inet_pton(AF_INET, "10.37.62.58", &saddr.sin_addr.s_addr);
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(9999);

    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1) {
        perror("bind");
        exit(-1);
    }

    // 3. 监听
    ret = listen(lfd, 8);
    if (ret == -1) {
        perror("listen");
        exit(-1);
    }

    // 4. 接收客户端连接
    struct sockaddr_in clientaddr;
    int clen = sizeof(clientaddr);
    int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &clen);
    if (cfd == -1) {
        perror("accept");
        exit(-1);
    }

    // 5. 输出客户端信息
    char clientIP[16];
    inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, clientIP, sizeof(clientIP));
    unsigned short clientPort = ntohs(clientaddr.sin_port);

    printf("client IP: %s, client Port: %d\n", clientIP, clientPort);

    // 获取客户端的数据
    char recvBuf[1024] = {0};
    while (1) {
        int len = read(cfd, recvBuf, sizeof(recvBuf));
        if (len == -1) {
            perror("read");
            exit(-1);
        }
        else if (len > 0) {
            printf("receive data: %s\n", recvBuf);
        }
        else if (len == 0) {
            // 客户端断开连接
            printf("client close\n");
            break;
        }

        // 给客户端发送数据
        char *data = "hello, I am server";
        write(cfd, data, strlen(data));
    }


    // 关闭文件描述符
    close(cfd);
    close(lfd);

    return 0;
}