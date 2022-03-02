#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <poll.h>

int main() {

    
    // 1. 创建socket
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in saddr;
    saddr.sin_port = htons(9999);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;

    // 2. 绑定
    bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));

    // 3. 监听
    listen(lfd, 8);

    // 初始化文件描述符集合
    struct pollfd fds[1024];
    for (int i = 0; i < 1024; i++) {
        fds[i].fd = -1;
        fds[i].events = POLLIN;
    }
    fds[0].fd = lfd;
    int nfds = 0;
    while (1) {

        // 调用poll系统函数，看是否有数据
        int ret = poll(fds, nfds + 1, -1);
        if (ret == -1) {
            perror("poll");
            exit(-1);
        } else if (ret == 0) {
            continue;
        } else if (ret > 0) {
            // 有文件描述符对应的缓冲区发生了变化
            if (fds[0].revents & POLLIN) {
                // 有客户端连接
                struct sockaddr_in cliaddr;
                int len = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &len);
                // 将新的文件描述符加入集合中
                for (int i = 1; i < 1024; i++) {
                    if (fds[i].fd == -1) {
                        fds[i].fd = cfd;
                        fds[i].events = POLLIN;
                        break;
                    }
                }
                // 最大文件描述符的索引
                nfds = nfds > cfd ? nfds : cfd;
            }

            for (int i = 1; i <= nfds; i++) {
                if (fds[i].revents & POLLIN) {
                    // 说明对应的客户端发来了数据
                    char buf[1024] = {0};
                    int len = read(fds[i].fd, buf, sizeof(buf));
                    if (len == -1) {
                        perror("read");
                        exit(-1);
                    }
                    else if (len == 0) {
                        printf("client close\n");
                        close(fds[i].fd);
                        fds[i].fd = -1;
                    }
                    else if (len > 0) {
                        printf("read buf: %s\n", buf);
                        write(fds[i].fd, buf, strlen(buf) + 1);
                    }
                }
            }
        }

    }

    close(lfd);

    return 0;
}