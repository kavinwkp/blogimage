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

    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = myalarm;
    sigemptyset(&act.sa_mask);  // 清空临时阻塞信号集

    sigaction(SIGALRM, &act, NULL);

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

    // getchar();
    while (1);

    return 0;
}
