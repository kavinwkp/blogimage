#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>

int main() {

    // 1. 创建共享内存
    int shmid = shmget(100, 0, IPC_CREAT);
    printf("shmid: %d\n", shmid);
    // 2. 和当前进程关联
    void *ptr = shmat(shmid, NULL, 0);

    // 3. 读数据
    printf("%s\n", (char *)ptr);

    printf("press any key to continue...\n");
    getchar();

    // 4. 解除关联
    shmdt(ptr);

    // 5. 删除共享内存
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}