#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {

    // 1. 创建socket
    int fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        perror("socket");
        exit(-1);
    }

    // 2. 设置广播属性
    int op = 1;
    setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &op, sizeof(op));

    // 3. 创建广播地址
    struct sockaddr_in cliaddr;
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(9999);
    inet_pton(AF_INET, "192.168.43.217", &cliaddr.sin_addr.s_addr);

    // 3. 通信
    int num = 0;
    while (1) {
        char sendbuf[128];
        sprintf(sendbuf, "hello, client %d\n", num++);

        sendto(fd, sendbuf, strlen(sendbuf) + 1, 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
        printf("broadcast: %d\n", num);
        sleep(1);
    }

    close(fd);


    return 0;
}