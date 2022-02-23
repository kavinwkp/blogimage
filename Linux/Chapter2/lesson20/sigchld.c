#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>

void myFun(int num) {
    printf("catch signal: %d\n", num);
    // wait(NULL);
    while (1) {
        int ret = waitpid(-1, NULL, WNOHANG);
        if (ret > 0) {
            printf("child die, id: %d\n", ret);
        }
        else if (ret == 0) {
            break;
        }
        else if (ret == -1) {
            break;
        }
    }
}

int main() {

    // 提前设置阻塞SIGCHLD
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    sigprocmask(SIG_BLOCK, &set, NULL);

    pid_t pid;
    for (int i = 0; i < 10; i++) {
        pid = fork();
        if (pid == 0) break;
    }

    if (pid > 0) {

        // 捕捉SIGCHLD信号
        struct sigaction act;
        act.sa_flags = 0;
        act.sa_handler = myFun;
        sigemptyset(&act.sa_mask);
        sigaction(SIGCHLD, &act, NULL);

        sigprocmask(SIG_UNBLOCK, &set, NULL);   // 解除阻塞

        while (1) {
            printf("parent id: %d\n", getpid());
            sleep(2);
        }
    } else if (pid == 0) {
        printf("child id: %d\n", getpid());
    }

    return 0;
}