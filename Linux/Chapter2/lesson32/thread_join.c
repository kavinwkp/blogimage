#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int val = 10;

void* callback(void* arg) {
    printf("child thread: %ld\n", pthread_self());  // 获取当前线程id
    // sleep(3);
    // return NULL;
    pthread_exit((void *)&val); // return (void *)&val
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

    // 主线程回收子线程的资源
    int *thread_retval;
    ret = pthread_join(tid, (void **)&thread_retval);  // 阻塞直至子线程结束
    if (ret != 0) {
        char *errstr = strerror(ret);
        printf("%s\n", errstr);
    }

    printf("return value: %d\n", *thread_retval);
    printf("recovery success\n");
    return 0;
}