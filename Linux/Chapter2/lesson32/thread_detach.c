#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void* callback(void* arg) {
    printf("child thread id: %ld\n", pthread_self());
    return NULL;
}

int main() {
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);
    if (ret != 0) {
        char *errstr = strerror(ret);
        printf("error: %s\n", errstr);
    }

    // 输出主线程和子线程id
    printf("tid: %ld, main id: %ld\n", tid, pthread_self());

    // 设置子线程分离
    pthread_detach(tid);

    pthread_exit(NULL);
    return 0;
}