#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void* callback(void* arg) {
    printf("child thread id: %ld\n", pthread_self());
    return NULL;
}

int main() {

    // 创建一个线程属性变量
    pthread_attr_t attr;
    // 初始化属性变量
    pthread_attr_init(&attr);
    // 设置属性
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_t tid;
    int ret = pthread_create(&tid, &attr, callback, NULL);
    if (ret != 0) {
        char *errstr = strerror(ret);
        printf("error: %s\n", errstr);
    }

    // 获取线程栈
    size_t size;
    pthread_attr_getstacksize(&attr, &size);
    printf("thread stack size: %ld\n", size);

    // 输出主线程和子线程id
    printf("tid: %ld, main id: %ld\n", tid, pthread_self());

    // 释放线程属性资源
    pthread_attr_destroy(&attr);

    pthread_exit(NULL);
    return 0;
}