#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// 3个子线程卖100张票
int tickets = 1000;

// 创建互斥量
pthread_mutex_t mutex;

void* sellticket(void *arg) {

    while (1) {
        // 加锁
        pthread_mutex_lock(&mutex);
        if (tickets > 0) {
            usleep(5000);
            printf("%ld is selling %d ticket\n", pthread_self(), tickets);
            tickets--;
        }
        else {
            // 解锁
            pthread_mutex_unlock(&mutex);
            break;
        }
        // 解锁
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main() {
    // 初始化互斥量
    pthread_mutex_init(&mutex, NULL);

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

    // 释放互斥量资源
    pthread_mutex_destroy(&mutex);

    return 0;
}