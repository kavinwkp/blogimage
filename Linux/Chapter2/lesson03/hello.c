#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main() {

    printf("begin\n");

    if (fork() > 0) {

        // 大于0，返回的是子进程的进程号，当前是父进程
        printf("I am parent, pid: %d, ppid: %d\n", getpid(), getppid());

        int i;
        for (i = 0; i < 10; i++) {
            printf("i = %d\n", i);
            sleep(1);
        }
    }
    else {
        // 等于0，当前是子进程
        printf("I am child, pid: %d, ppid: %d\n", getpid(), getppid());
        int j;
        for (j = 0; j < 10; j++) {
            printf("j = %d\n", j);
            sleep(1);
        }

    }
    return 0;
}