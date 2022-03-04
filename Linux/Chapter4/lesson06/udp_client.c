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

        // 2. 绑定
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr.s_addr);

    int num = 0;
    while (1) {
        char sendbuf[128];
        sprintf(sendbuf, "hello, I am client %d\n", num++);
        sendto(fd, sendbuf, strlen(sendbuf) + 1, 0, (struct sockaddr*)&saddr, sizeof(saddr));
        
        int num = recvfrom(fd, sendbuf, sizeof(sendbuf), 0, NULL, NULL);
        printf("server: %s\n", sendbuf);

        sleep(1);
    }
    close(fd);

    return 0;
}