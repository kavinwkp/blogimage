#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// 3个子线程卖100张票
int tickets = 100;

void* sellticket(void *arg) {
    while (tickets > 0) {
        usleep(5000);
        printf("%ld is selling %d ticket\n", pthread_self(), tickets);
        tickets--;
    }
    return NULL;
}

int main() {
    // 创建3个子线程
    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, sellticket, NULL);
    pthread_create(&tid2, NULL, sellticket, NULL);
    pthread_create(&tid3, NULL, sellticket, NULL);
    // 回收子线程的资源
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    // 退出主线程
    pthread_exit(NULL);

    return 0;
}