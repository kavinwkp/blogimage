#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

struct sockInfo {
    int fd; // 通信的文件描述符
    struct sockaddr_in addr;    // 客户端信息
    pthread_t tid;  // 线程号
};

struct sockInfo sockinfos[128];

void* working(void *arg) {
    // 子线程和客户端通信
    struct sockInfo *pinfo = (struct sockInfo *)arg;
    char cliIP[16];
    inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, cliIP, sizeof(cliIP));
    unsigned short cliPort = ntohs(pinfo->addr.sin_port);
    printf("client IP: %s, Port: %d\n", cliIP, cliPort);

    // 接收客户端数据
    char recvBuf[1024];
    while (1) {
        int len = read(pinfo->fd, recvBuf, sizeof(recvBuf));
        if (len == -1) {
            perror("read");
            exit(-1);
        }
        else if (len > 0) {
            printf("receive %s\n", recvBuf);
        }
        else if (len == 0) {
            printf("client close\n");
            break;
        }
        write(pinfo->fd, recvBuf, strlen(recvBuf) + 1);   // 带上字符串结束符
    }
    close(pinfo->fd);
    return NULL;
}

int main() {
    // 1. 创建用于监听的套接字
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    if (lfd == -1) {
        perror("socket");
        exit(-1);
    }
    // 2. 绑定
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    saddr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1) {
        perror("bind");
        exit(-1);
    }

    // 3. 监听
    ret = listen(lfd, 128);
    if (ret == -1) {
        perror("listen");
        exit(-1);
    }

    // 初始化数据
    int max = sizeof(sockinfos) / sizeof(sockinfos[0]); 
    for (int i = 0; i < max; i++) {
        bzero(&sockinfos[i], sizeof(sockinfos[i]));
        sockinfos[i].fd = -1;
        sockinfos[i].tid = -1;
    }

    // 4. 循环等待客户端连接，创建子线程
    while (1) {
        struct sockaddr_in cliaddr;
        int len = sizeof(cliaddr);
        int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);
        // if (cfd == -1) {
        //     if (errno == EINTR) {
        //         continue;
        //     }
        //     perror("accept");
        //     exit(-1);
        // }

        struct sockInfo *pInfo;
        for (int i = 0; i < max; i++) {
            // 从数组中找到一个可用的sockInfo元素
            if (sockinfos[i].fd == -1) {
                pInfo = &sockinfos[i];
                break;
            }
            if (i == max - 1) {
                sleep(1);
                i--;
            }
        }
        pInfo->fd = cfd;
        memcpy(&pInfo->addr, &cliaddr, len);
        // 创建子线程
        pthread_create(&pInfo->tid, NULL, working, pInfo);
        // 线程分离
        pthread_detach(pInfo->tid);
    }

    close(lfd);

    return 0;
}