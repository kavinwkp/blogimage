// TCP通信的服务器端

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

void recycleChild(int arg) {
    while (1) {
        int ret = waitpid(-1, NULL, WNOHANG);
        if (ret == -1) {
            // 所有子进程都回收了
            break;
        }
        else if (ret == 0) {
            // 还有子进程活着
            break;
        }
        else if (ret > 0) {
            printf("recycle child process: %d\n", ret);
        }
    }
}

int main() {
    // 注册信号捕捉回收子进程资源
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = recycleChild;
    sigemptyset(&act.sa_mask);
    sigaction(SIGCHLD, &act, NULL);

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

    // 4. 循环等待接收客户端连接
    while (1) {
        struct sockaddr_in cliaddr;
        int len = sizeof(cliaddr);
        int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);
        if (cfd == -1) {
            if (errno == EINTR) {
                continue;
            }
            perror("accept");
            exit(-1);
        }
        // 来一个连接就创建一个子进程
        pid_t pid = fork();
        if (pid == 0) {
            // 子进程
            // 获取客户端的信息
            char cliIP[16];
            inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, cliIP, sizeof(cliIP));
            unsigned short cliPort = ntohs(cliaddr.sin_port);
            printf("client IP: %s, Port: %d\n", cliIP, cliPort);

            // 接收客户端数据
            char recvBuf[1024];
            while (1) {
                int len = read(cfd, recvBuf, sizeof(recvBuf));
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
                write(cfd, recvBuf, strlen(recvBuf) + 1);   // 带上字符串结束符
            }
            close(cfd);
            exit(0);    // 子进程退出
        }
    }

    close(lfd);
    return 0;
}