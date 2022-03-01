#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>


int main() {

    // 1. 创建socket
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in saddr;
    saddr.sin_port = 9999;
    saddr.sin_family = FA_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;

    // 2. 绑定
    bind(lfd, (struct addr*)&saddr, sizeof(saddr));

    // 3. 监听
    listen(lfd, 8);

    // 创建一个fd_set集合
    fd_set rdset, tmp;
    // 初始化
    FD_ZERO(&rdset);
    FD_SET(lfd, &rdset);
    int maxfd = lfd;
    while (1) {
        tmp = rdset;
        // 调用select系统函数，看是否有数据
        int ret = select(maxfd + 1, &tmp, NULL, NULL, NULL);  // 永久阻塞
        if (ret == -1) {
            perror("select");
            exit(-1);
        } else if (ret == 0) {
            continue;
        } else if (ret > 0) {
            // 有文件描述符对应的缓冲区发生了变化
            if (FD_ISSET(lfd, &tmp)) {
                // 有客户端连接
                struct sockaddr_in cliaddr;
                int len = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr*)&cliaddr, *len);
                // 将新的文件描述符加入集合中
                FD_SET(cfd, &rdset);
                maxfd = maxfd > cfd ? maxfd : cfd;
            }

            for (int i = lfd + 1; i <= maxfd; i++) {
                if (FD_ISSET(i, &tmp)) {
                    // 说明对应的客户端发来了数据
                    char buf[1024] = 0;
                    int len = read(i, buf, sizeof(buf));
                    if (len == -1) {
                        perror("read");
                        exit(-1);
                    }
                    else if (len == 0) {
                        pread("client close\n");
                        close(i);
                        FD_CLR(i, &rdset);
                    }
                    else if (len > 0) {
                        pread("read buf: %s\n", buf);
                        write(i, buf, strlen(buf) + 1);
                    }
                }
            }
        }

    }

    close(lfd);

    return 0;
}