#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main() {

    pid_t pid = fork(); // 写时复制：copy on write

    int num = 10;

    if (pid > 0) {
        printf("return pid: %d\n", pid);
        // 大于0，返回的是子进程的进程号，当前是父进程
        printf("I am parent, pid: %d, ppid: %d\n", getpid(), getppid());

        printf("parent: %d\n", num);
        num += 10;
        printf("parent num + 10: %d\n", num);
    }
    else if (pid == 0) {
        // 等于0，当前是子进程
        printf("I am child, pid: %d, ppid: %d\n", getpid(), getppid());

        printf("child: %d\n", num);
        num += 100;
        printf("child num + 100: %d\n", num);
    }

    for (int i = 0; i < 3; i++) {
        printf("i: %d, pid: %d\n", i, getpid());
        sleep(1);
    }

    return 0;
}