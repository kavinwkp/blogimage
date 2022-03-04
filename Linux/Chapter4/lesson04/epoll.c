#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/epoll.h>

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

    // 4. 创建epoll实例
    int epfd = epoll_create(1);

    // 将要监听的文件描述符添加到文件描述符中
    struct epoll_event epev;
    epev.events = EPOLLIN;
    epev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &epev);

    struct epoll_event epevs[1024];
    while (1) {
        int ret = epoll_wait(epfd, epevs, 1024, -1);
        if (ret == -1) {
            perror("epoll");
            exit(-1);
        }
        printf("ret: %d\n", ret);
        for (int i = 0; i < ret; i++) {
            int curfd = epevs[i].data.fd;
            if (curfd == lfd) {
                // 监听到有客户端连接
                struct sockaddr_in cliaddr;
                int len = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &len);
                epev.events = EPOLLIN;
                epev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epev);
            }
            else {
                if (epevs[i].events & EPOLLOUT) {
                    continue;   // 写事件不需要操作
                }
                // 客户端数据到达
                char buf[1024] = {0};
                int len = read(curfd, buf, sizeof(buf));
                if (len == -1) {
                    perror("read");
                    exit(-1);
                }
                else if (len == 0) {
                    printf("client close\n");
                    epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);
                    close(curfd);
                }
                else if (len > 0) {
                    printf("read buf: %s\n", buf);
                    write(curfd, buf, strlen(buf) + 1);
                }
            }
        }
    }

    close(lfd);
    close(epfd);

    return 0;
}
