#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

// 过3秒后，每隔2秒定时一次


void myalarm(int num) {
    printf("catch signal: %d\n", num);
    printf("-----\n");
}

int main() {

    // signal(SIGALRM, SIG_DFL);
    signal(SIGALRM, myalarm);

    struct itimerval new_value;
    new_value.it_interval.tv_sec = 2;   // 时间间隔
    new_value.it_interval.tv_usec = 0;
    new_value.it_value.tv_sec = 3;  // 延迟时间
    new_value.it_value.tv_usec = 0;
    int ret = setitimer(ITIMER_REAL, &new_value, NULL);
    printf("timer start\n");
    if (ret == -1) {
        perror("setitimer");
        exit(0);
    }

    getchar();

    return 0;
}
