#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main() {

    pid_t pid = fork(); // 写时复制：copy on write

    if (pid > 0) {
        printf("I am parent, pid: %d, ppid: %d\n", getpid(), getppid());
    }
    else if (pid == 0) {
        sleep(1);
        printf("I am child, pid: %d, ppid: %d\n", getpid(), getppid());
    }

    for (int i = 0; i < 3; i++) {
        printf("i: %d, pid: %d\n", i, getpid());
    }

    return 0;
}