#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/un.h>

int main() {
    unlink("server.sock");
    // 1. 创建监听的套接字
    int lfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (lfd == -1) {
        perror("socket");
        exit(-1);
    }
    // 2. 绑定本地套接字
    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, "server.sock");
    int ret = bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret == -1) {
        perror("bind");
        exit(-1);
    }

    // 3. 监听
    ret = listen(lfd, 100);
    if (ret == -1) {
        perror("bind");
        exit(-1);
    }

    // 4. 等待客户端的连接
    struct sockaddr_un cliaddr;
    int len = sizeof(cliaddr);
    int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &len);
    if (cfd == -1) {
        perror("accept");
        exit(-1);
    }
    printf("socket file name: %s\n", cliaddr.sun_path);

    // 5. 通信
    while (1) {
        char buf[128];
        len = recv(cfd, buf, sizeof(buf), 0);
        if (len == -1) {
            perror("recv");
            exit(-1);
        } else if (len == 0) {
            printf("client close...\n");
            break;
        } else if (len > 0) {
            printf("client data: %s\n", buf);
            send(cfd, buf, len, 0);
        }
    }
    close(cfd);
    close(lfd);
    return 0;
}