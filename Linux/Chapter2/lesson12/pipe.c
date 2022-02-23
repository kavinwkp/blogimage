#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

int main() {

    // 在fork之前创建管道
    int pipefd[2];
    int ret = pipe(pipefd);
    if (ret == -1) {
        perror("pipe");
        exit(0);
    }

    pid_t pid = fork();
    if (pid > 0) {
        printf("I am parent, pid: %d\n", getpid());
        close(pipefd[1]);   // 关闭写端
        // 从管道读取端读取数据
        char buf[1024] = {0};
        while (1) {
            int len = read(pipefd[0], buf, sizeof(buf));
            printf("parent rev: %s, pid: %d\n", buf, getpid());

            // char *str = "hello, I am parent";
            // write(pipefd[1], str, strlen(str));
            // sleep(1);

        }
        
    }
    else if (pid == 0) {
        printf("I am child, pid: %d\n", getpid());
        close(pipefd[0]);   // 关闭读端
        char buf[1024] = {0};
        while (1) {
            char *str = "hello, I am child";
            write(pipefd[1], str, strlen(str));
            // sleep(1);

            // int len = read(pipefd[0], buf, sizeof(buf));
            // printf("chlid rev: %s, pid: %d\n", buf, getpid());
            // bzero(buf, 1024);
        }
        
    }

    return 0;
} 