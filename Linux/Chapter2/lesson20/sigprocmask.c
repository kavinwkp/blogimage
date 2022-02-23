#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    // 设置2 3 信号阻塞
    sigset_t sigset;
    sigemptyset(&sigset);
    // 将2 3信号添加到信号集中
    sigaddset(&sigset, SIGINT);
    sigaddset(&sigset, SIGQUIT);

    // 修改内核中的阻塞信号集
    sigprocmask(SIG_BLOCK, &sigset, NULL);

    int num = 0;
    while (1) {
        num++;
        sigset_t pendingset;
        sigemptyset(&pendingset);
        sigpending(&pendingset);
        for (int i = 1; i <= 32; i++) {
            if ((sigismember(&pendingset, i)) == 1) {
                printf("1");
            }
            else if ((sigismember(&pendingset, i)) == 0) {
                printf("0");
            }
            else {
                perror("sigpending");
                exit(0);
            }
        }
        printf("\n");
        sleep(1);
        if (num == 10) {
            // 解除阻塞
            sigprocmask(SIG_UNBLOCK, &sigset, NULL);
        }
    }

    return 0;
}
00000000000000000000000000000000
00000000000000000000000000000000
^C01000000000000000000000000000000
01000000000000000000000000000000
01000000000000000000000000000000
^\01100000000000000000000000000000
01100000000000000000000000000000
01100000000000000000000000000000
01100000000000000000000000000000
01100000000000000000000000000000
