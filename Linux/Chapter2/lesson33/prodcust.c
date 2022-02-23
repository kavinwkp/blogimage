// 生产者-消费者模型

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
struct Node {
    int num;
    struct Node *next;
};

struct Node *head = NULL;

// 创建互斥量
pthread_mutex_t mutex; 

void* producer(void *arg) {

    while (1) {
        pthread_mutex_lock(&mutex);
        // 创建新的节点，添加到链表中
        struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->next = head;
        head = newNode;
        newNode->num = rand() % 100;
        printf("add Node, num: %d, tid: %ld\n", newNode->num, pthread_self());

        pthread_mutex_unlock(&mutex);
        usleep(100);
    }
    return NULL;
}

void* customer(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        struct Node *tmp = head;
        if (head != NULL) {
            // 有数据
            head = head->next;
            printf("delete node, num: %d, tid: %ld\n", tmp->num, pthread_self());
            free(tmp);
            pthread_mutex_unlock(&mutex);
        }
        else {
            // 没有数据
            pthread_mutex_unlock(&mutex);
        }
        
        usleep(100);
    }
    return NULL;
}

int main() {
    pthread_mutex_init(&mutex, NULL);
    // 创建5个生产者，5个消费者
    pthread_t ptids[5], ctids[5];
    for (int i = 0; i < 5; i++) {
        pthread_create(&ptids[i], NULL, producer, NULL);
        pthread_create(&ctids[i], NULL, customer, NULL);
    }

    // 线程分离
    for (int i = 0; i < 5; i++) {
        pthread_detach(ptids[i]);
        pthread_detach(ctids[i]);
    }

    while (1) {
        sleep(10);
    }

    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);

    return 0;
}