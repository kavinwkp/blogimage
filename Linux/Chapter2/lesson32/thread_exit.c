#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* callback(void* arg) {
    printf("child thread: %ld\n", pthread_self());  // 获取当前线程id
    return NULL;
}

int main() {
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);
    if (ret != 0) {
        char *errstr = strerror(ret);
        printf("%s\n", errstr);
    }
    // 主线程
    for (int i = 0; i < 5; i++) {
        printf("%d\n", i);
    }
    printf("tid: %ld, main id: %ld\n", tid, pthread_self());
    // 主线程退出，对子线程没有影响
    pthread_exit(NULL);

    return 0;
}